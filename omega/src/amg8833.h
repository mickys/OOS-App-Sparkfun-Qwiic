#if !defined(AMG8833_H_INCLUDED)
#define AMG8833_H_INCLUDED

#define AMG8833_I2C_DEV_NUM 		0
#define AMG8833_I2C_DEV_ADDR 		0x69

// register definitions
#define AMG8833_PCTL 		0x00
#define AMG8833_RST 		0x01
#define AMG8833_FPSC 		0x02
#define AMG8833_INTC 		0x03

// constants
#define AMG8833_PIXEL_TEMP_CONVERSION 	0.25
#define AMG8833_SLEEP_MS 				100
#define AMG8833_NUM_PIXELS              64


// functions
int amg8833_setup();
int amg8833_readPixels(float *pixelData);


#endif	/* AMG8833_H_INCLUDED */
