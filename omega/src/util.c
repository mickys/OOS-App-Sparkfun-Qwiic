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

#if UTIL_DEBUG == 1
    printf("readRegister16: read from 0x%02x: 0x%02x, 0x%02x\n", addr, rdBuffer[1], rdBuffer[0]);
#endif // UTIL_DEBUG
    
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

/// 16 bit address functions ///
uint8_t addr16_readRegister(uint8_t devAddr, uint16_t addr) {
    int status, addrLength, dataLength;
    uint8_t value;
    addrLength = 2;
    dataLength = 1;
    uint8_t *addrBuffer = malloc(addrLength * sizeof(uint8_t));
    uint8_t *rdBuffer = malloc(dataLength * sizeof(uint8_t));

    // split the address into bytes
    addrBuffer[1] = addr & 0xff;
    addrBuffer[0] = (addr >> 8) & 0xff;
    
    // perform the read
    status = i2c_readMultiByteAddr(OMEGA_I2C_DEV_NUM, devAddr, addrBuffer, addrLength, rdBuffer, dataLength);
#if UTIL_DEBUG == 1
    printf("addr16_readRegister: read (device 0x%02x) from 0x%04x: 0x%02x\n", devAddr, addr, rdBuffer[0]);
#endif // UTIL_DEBUG

    value = rdBuffer[0];
    free(addrBuffer);
    free(rdBuffer);
    return value;
}

uint16_t addr16_readRegister16(uint8_t devAddr, uint16_t addr) {
    int status, length;
    uint16_t value;
    length = 2;
    uint8_t *addrBuffer = malloc(length * sizeof(uint8_t));
    uint8_t *rdBuffer = malloc(length * sizeof(uint8_t));
    
    addrBuffer[1] = addr & 0xff;
    addrBuffer[0] = (addr >> 8) & 0xff;
    
    status = i2c_readMultiByteAddr(OMEGA_I2C_DEV_NUM, devAddr, addrBuffer, length, rdBuffer, length);

#if UTIL_DEBUG == 1
    printf("addr16_readRegister16: read (device 0x%02x) from 0x%04x: 0x%02x, 0x%02x\n", devAddr, addr, rdBuffer[1], rdBuffer[0]);
#endif // UTIL_DEBUG

    value = ( ((uint16_t)rdBuffer[0] << 8) | rdBuffer[1] );
    free(rdBuffer);
    return value;
}

int addr16_writeRegister(uint8_t devAddr, uint16_t addr, uint8_t data) {
    int status, addrLength, dataLength;
    uint16_t value;
    addrLength = 2;
    dataLength = 1;
    uint8_t *addrBuffer = malloc(addrLength * sizeof(uint8_t));
    uint8_t *wrBuffer = malloc(dataLength * sizeof(uint8_t));
    
    // split the address into bytes
    addrBuffer[1] = addr & 0xff;
    addrBuffer[0] = (addr >> 8) & 0xff;
    // move the data to the buffer
    wrBuffer[0] = data;
    
    // perform the write 
    status = i2c_writeMultiByteAddr(OMEGA_I2C_DEV_NUM, devAddr, addrBuffer, addrLength, wrBuffer, dataLength);

#if UTIL_DEBUG == 1
    printf("addr16_writeRegister16: write (device 0x%02x) to addr 0x%04x: 0x%02x, 0x%02x\n", devAddr, addr, wrBuffer[1], wrBuffer[0]);
#endif // UTIL_DEBUG
    
    free(addrBuffer);
    free(wrBuffer);
    return status;
}

int addr16_writeRegister16(uint8_t devAddr, uint16_t addr, uint16_t data) {
    int status, length;
    uint16_t value;
    length = 2;
    uint8_t *addrBuffer = malloc(length * sizeof(uint8_t));
    uint8_t *wrBuffer = malloc(length * sizeof(uint8_t));
    
    // split the address into bytes
    addrBuffer[1] = addr & 0xff;
    addrBuffer[0] = (addr >> 8) & 0xff;
    // split the data into bytes
    wrBuffer[1] = data & 0xff;
    wrBuffer[0] = (data >> 8) & 0xff;
    
    // perform the write 
    status = i2c_writeMultiByteAddr(OMEGA_I2C_DEV_NUM, devAddr, addrBuffer, length, wrBuffer, length);

#if UTIL_DEBUG == 1
    printf("addr16_writeRegister16: write (device 0x%02x) to addr 0x%04x: 0x%02x, 0x%02x\n", devAddr, addr, wrBuffer[1], wrBuffer[0]);
#endif // UTIL_DEBUG
    
    free(addrBuffer);
    free(wrBuffer);
    return status;
}
