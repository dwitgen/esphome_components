#ifndef _ES8311_DEBUG_H_
#define _ES8311_DEBUG_H_

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

// Debug functions for ES8311 codec
void debug_es8311_i2c(void);
void verify_es8311_config(void);
esp_err_t reset_i2c_driver(void);

#ifdef __cplusplus
}
#endif

#endif // _ES8311_DEBUG_H_
