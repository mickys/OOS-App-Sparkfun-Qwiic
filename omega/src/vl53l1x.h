#if !defined(_VL53L1X_H_INCLUDED)
#define _VL53L1X_H_INCLUDED

#include <stdint.h>


#define VL53L1X_DEBUG                0

#define VL53L1X_DEV_NAME            "VL53L1X"

#define VL53L1X_I2C_DEV_NUM 		0
#define VL53L1X_I2C_DEV_ADDR 		0x29

// register definitions
#define VL53L1_SOFT_RESET                                                                0x0000

#define VL53L1_PAD_I2C_HV__EXTSUP_CONFIG                                                 0x002E

#define VL53L1_GPIO__TIO_HV_STATUS                                                       0x0031
#define VL53L1_RESULT__FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD0                            0x0096
#define VL53L1_FIRMWARE__SYSTEM_STATUS                                                   0x00E5


#define VL53L1_IDENTIFICATION__MODEL_ID                                                  0x010F

// constants
#define VL53L1X_SLEEP_MS 		50


int         vl53l1x_setup               ();

int         vl53l1x_newDataReady        ();
uint16_t    vl53l1x_getDistance         ();

#endif	/* _VL53L1X_H_INCLUDED */