#include "util.h"

#include <onion-i2c.h>


uint8_t readRegisterReturn(uint8_t devAddr, uint8_t addr)
{
    int status;
    int val;
    status = i2c_readByte (OMEGA_I2C_DEV_NUM, devAddr, addr, &val);
    return (uint8_t)val;
}

uint16_t readRegister16(uint8_t devAddr, uint8_t addr) {
    uint8_t rdBuffer[2];
    int status, length;
    length = 2;
    status = i2c_read(OMEGA_I2C_DEV_NUM, devAddr, addr, rdBuffer, length);  
    return ( ((uint16_t)rdBuffer[1] << 8) | rdBuffer[0] );
}

int writeRegister(uint8_t devAddr, uint8_t addr, uint8_t data)
{
    int status;
    status = i2c_write(OMEGA_I2C_DEV_NUM, devAddr, addr, (uint8_t)data);
    return status;
}

int writeRegister16(uint8_t devAddr, uint8_t addr, uint16_t data)
{
    int status, length;
    length = 2;
    uint8_t *buffer = malloc(length * sizeof(uint8_t));
    
    buffer[0] = data & 0xff;
    buffer[1] = (data >> 8) & 0xff;
    
    status = i2c_writeBuffer(OMEGA_I2C_DEV_NUM, devAddr, addr, buffer, length);
    return status;
}