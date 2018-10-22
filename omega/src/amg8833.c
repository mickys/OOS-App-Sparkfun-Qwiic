#include "amg8833.h"
#include <stdint.h>
#include <stdlib.h>
#include <onion-i2c.h>
#include <stdio.h>	// tmp while we printf debug

int amg8833_setup() {
	int status;

	// perform AMG8833 setup
	status = i2c_write(AMG8833_I2C_DEV_NUM, AMG8833_I2C_DEV_ADDR, AMG8833_PCTL, (uint8_t)0x00);	// normal mode
	status |= i2c_write(AMG8833_I2C_DEV_NUM, AMG8833_I2C_DEV_ADDR, AMG8833_RST, (uint8_t)0x3f);	// software reset
	status |= i2c_write(AMG8833_I2C_DEV_NUM, AMG8833_I2C_DEV_ADDR, AMG8833_INTC, (uint8_t)0x00);	// disable interrupts
	status |= i2c_write(AMG8833_I2C_DEV_NUM, AMG8833_I2C_DEV_ADDR, AMG8833_FPSC, (uint8_t)0x00);	// 10 fps

	usleep(AMG8833_SLEEP_MS * 1000);

	return status;
}

// two's complement 
/* 	def twoCompl12(self, val):
		if  0x7FF & val == val:
			return float(val)
		else:
			return float(val-4096 )*/
			
uint16_t twoComplement(uint16_t input) {
	if (0x7ff & input == input) {
		return input;
	} else {
		return (uint16_t)(input-4096);
	}
}
			

int amg8833_readPixels(float *pixelData) {
	uint8_t	*rdBuffer = malloc(2 * sizeof(uint8_t));
	float val; 

	for (int i = 0; i < AMG8833_NUM_PIXELS; i++) {
		// read two bytes at a time
		i2c_read(AMG8833_I2C_DEV_NUM, AMG8833_I2C_DEV_ADDR, i*2 + 128, rdBuffer, 2);
		val = (float)twoComplement( ((rdBuffer[1] & 0xff) << 8) | (rdBuffer[0] & 0xff) ) * AMG8833_PIXEL_TEMP_CONVERSION;
		pixelData[i] = val;
		
		// printf("%d: 0x%02x 0x%02x,   %.01f\n", i, rdBuffer[1], rdBuffer[0], val);
		// printf("%.01f\n", val);
	}


	free(rdBuffer);
	return EXIT_SUCCESS;
}

void amg8833_generateJsonArray(float *pixelData, char *json) {
	int pos = 0;
	pos += sprintf(&json[pos], "[%.01f", pixelData[0]);
	for (int i = 1; i < AMG8833_NUM_PIXELS; i++) {
		pos += sprintf(&json[pos], ", %.01f", pixelData[i]);
		// sprintf(json, "%s%.01f", json, pixelData[i]);
		// if (i < (AMG8833_NUM_PIXELS-1)) {
			// strcat(json, ",");
		// }
	}
	strcat(json, "]");
}