#include <stdlib.h>

#include <onion-i2c.h>
#include "ccs811.h"

int devAddr;


int multiReadRegister(uint8_t addr, uint8_t *buffer, uint8_t length) {
    int status;
    status = i2c_read(CCS811_I2C_DEV_NUM, devAddr, addr, buffer, length);
    return status;
}

int multiWriteRegister(uint8_t addr, uint8_t *buffer, uint8_t length) {
    int status;
    status = i2c_writeBuffer(CCS811_I2C_DEV_NUM, devAddr, addr, buffer, length);
    return status;
}

///// API FUNCTIONS /////
int ccs811_setup() {
    int status;
    uint8_t data[4] = {0x11,0xE5,0x72,0x8A}; //Reset key
    
    devAddr = CCS811_I2C_DEV_ADDR;
    
    // reset the device
    status = multiWriteRegister(CCS811_SW_RESET, data, 4);
    // allow sensor to reset
    usleep(CCS811_SLEEP_MS * 1000);
    
    // start the sensor
    uint8_t start[1] = {CCS811_APP_START};
    status = i2c_writeBufferRaw	(CCS811_I2C_DEV_NUM, devAddr, start, 1);
    
    // allow sensor to start
    usleep(CCS811_SLEEP_MS * 1000);
    
    return status;
}