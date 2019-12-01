#if !defined(_ENV_COMBO_H_INCLUDED)
#define _ENV_COMBO_H_INCLUDED

#include <stdint.h>


#define ENV_COMBO_DEV_NAME            "ENV_COMBO"
#define ENV_BME_DEV_NAME              "ENV_BME"

#define ENV_COMBO_I2C_DEV_NUM 		0
#define ENV_COMBO_I2C_DEV_ADDR 		0x69

// register definitions
#define BME280_CONFIG 		

#define ENV_COMBO_RST 		0x01
#define ENV_COMBO_FPSC 		0x02
#define ENV_COMBO_INTC 		0x03

// constants
#define ENV_COMBO_PIXEL_TEMP_CONVERSION 	0.25
#define ENV_COMBO_SLEEP_MS 				    1000
#define ENV_COMBO_NUM_PIXELS              64


// functions
int envComboSetup();
int envBmeSetup();

int envComboRead(float *temp, float *humidity, float *pressure, uint16_t *CO2, uint16_t *tVOC);
int envBmeRead(float *temp, float *humidity, float *pressure);
int envReadDS18(const char* ds18cmd, float* ds18b20);
void envComboGenerateJson(char *json, float temp, float humidity, float pressure, uint16_t CO2, uint16_t tVOC, const char* identifier, float ds18b20);


#endif	/* _ENV_COMBO_H_INCLUDED */
