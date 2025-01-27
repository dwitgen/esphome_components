#include "driver/i2c.h"
#include "esp_log.h"
#include <string.h>
#include "es8311.h"
#include "es8311_debug.h"
#include "board.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define TAG "ES8311_DEBUG"
#define ES8311_I2C_ADDRESS 0x40  // Update if your address differs

void debug_es8311_i2c() {
    uint8_t registers_to_test[] = {0x00, 0x01, 0x1B, 0x45}; // Common readable registers
    uint8_t data;
    esp_err_t ret;

    ESP_LOGI(TAG, "Testing I2C communication with ES8311...");
    for (int i = 0; i < sizeof(registers_to_test); i++) {
        uint8_t reg = registers_to_test[i];
        ret = i2c_master_write_read_device(
            I2C_NUM_0, ES8311_I2C_ADDRESS, &reg, 1, &data, 1, pdMS_TO_TICKS(100)
        );

        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "Register 0x%02X = 0x%02X", reg, data);
        } else {
            ESP_LOGE(TAG, "Failed to read register 0x%02X: %s", reg, esp_err_to_name(ret));
        }
    }
}


void verify_es8311_config() {
    uint8_t test_register = 0x10;  // Example writable register
    uint8_t test_value = 0x55;     // Test pattern
    esp_err_t ret;

    ESP_LOGI(TAG, "Testing write-then-read for ES8311...");

    // Write test
    ret = i2c_master_write_to_device(
        I2C_NUM_0, ES8311_I2C_ADDRESS, (uint8_t[]){test_register, test_value}, 2, pdMS_TO_TICKS(100)
    );
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Write test passed: Register 0x%02X set to 0x%02X", test_register, test_value);
    } else {
        ESP_LOGE(TAG, "Write test failed: Register 0x%02X, error: %s", test_register, esp_err_to_name(ret));
        return;
    }

    // Read back test
    uint8_t read_back_value;
    ret = i2c_master_write_read_device(
        I2C_NUM_0, ES8311_I2C_ADDRESS, &test_register, 1, &read_back_value, 1, pdMS_TO_TICKS(100)
    );
    if (ret == ESP_OK && read_back_value == test_value) {
        ESP_LOGI(TAG, "Read-back test passed: Register 0x%02X = 0x%02X", test_register, read_back_value);
    } else if (ret == ESP_OK) {
        ESP_LOGE(TAG, "Read-back mismatch: Register 0x%02X = 0x%02X (expected: 0x%02X)", 
                 test_register, read_back_value, test_value);
    } else {
        ESP_LOGE(TAG, "Read-back test failed: Register 0x%02X, error: %s", test_register, esp_err_to_name(ret));
    }
}

esp_err_t reset_i2c_driver() {
    esp_err_t ret = i2c_driver_delete(I2C_NUM_0);  // Replace I2C_NUM_0 with your I2C port number
    if (ret == ESP_OK) {
        ESP_LOGI("I2C_DRIVER", "I2C driver successfully cleared.");
    } else if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGW("I2C_DRIVER", "I2C driver already cleared or not installed.");
    } else {
        ESP_LOGE("I2C_DRIVER", "Failed to clear I2C driver: %s", esp_err_to_name(ret));
    }
    return ret;
}
