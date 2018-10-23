#if !defined(_CCS811_H_INCLUDED)
#define _CCS811_H_INCLUDED

#define CCS811_DEBUG                0

#define CCS811_DEV_NAME            "CCS811"

#define CCS811_I2C_DEV_NUM 		    0
#define CCS811_I2C_DEV_ADDR 		0x5B

// register definitions
#define CCS811_STATUS               0x00
#define CCS811_MEAS_MODE            0x01
#define CCS811_ALG_RESULT_DATA      0x02
#define CCS811_RAW_DATA             0x03
#define CCS811_ENV_DATA             0x05
#define CCS811_NTC                  0x06
#define CCS811_THRESHOLDS           0x10
#define CCS811_BASELINE             0x11
#define CCS811_HW_ID                0x20
#define CCS811_HW_VERSION           0x21
#define CCS811_FW_BOOT_VERSION      0x23
#define CCS811_FW_APP_VERSION       0x24
#define CCS811_ERROR_ID             0xE0
#define CCS811_APP_START            0xF4
#define CCS811_SW_RESET             0xFF

// constants

#define CCS811_SLEEP_MS 		2



int     ccs811_setup        ();

uint8_t ccs811_getErrorRegister         ( void );
uint8_t ccs811_checkStatus              ( void );

int     ccs811_dataAvailable            ( void );

int     ccs811_readAlgorithmResults     ( uint16_t *CO2, uint16_t *tVOC );

int     ccs811_setEnvironmentalData     ( float relativeHumidity, float temperature );

#endif	/* _CCS811_H_INCLUDED */