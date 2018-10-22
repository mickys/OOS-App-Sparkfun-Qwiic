#include "amg8833.h"
#include <stdint.h>
#include <stdlib.h>
// #include <onion-i2c.h>

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

int amg8833_readPixels() {
	uint8_t	*rdBuffer = malloc(2 * sizeof(uint8_t));


	for (int i = 0; i < 128; i+=2) {
		// read two bytes at a time
		i2c_read(AMG8833_I2C_DEV_NUM, AMG8833_I2C_DEV_ADDR, i + 128, rdBuffer, 2);
		printf("0x%02x 0x%02x\n", rdBuffer[0], rdBuffer[1]);
	}


	free(rdBuffer);
	return EXIT_SUCCESS;
}
