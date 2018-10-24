#include <stdlib.h>
#include <unistd.h>

#include <onion-i2c.h>

#include "vl53l1x.h"
#include "util.h"

int devAddr;

uint8_t configBlock[] = {
    0x29, 0x02, 0x10, 0x00, 0x28, 0xBC, 0x7A, 0x81, //8
    0x80, 0x07, 0x95, 0x00, 0xED, 0xFF, 0xF7, 0xFD, //16
    0x9E, 0x0E, 0x00, 0x10, 0x01, 0x00, 0x00, 0x00, //24
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x00, //32
    0x28, 0x00, 0x0D, 0x0A, 0x00, 0x00, 0x00, 0x00, //40
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, //48
    0x02, 0x00, 0x02, 0x08, 0x00, 0x08, 0x10, 0x01, //56
    0x01, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x02, //64
    0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x0B, 0x00, //72
    0x00, 0x02, 0x0A, 0x21, 0x00, 0x00, 0x02, 0x00, //80
    0x00, 0x00, 0x00, 0xC8, 0x00, 0x00, 0x38, 0xFF, //88
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x91, 0x0F, //96
    0x00, 0xA5, 0x0D, 0x00, 0x80, 0x00, 0x0C, 0x08, //104
    0xB8, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x10, 0x00, //112
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0F, //120
    0x0D, 0x0E, 0x0E, 0x01, 0x00, 0x02, 0xC7, 0xFF, //128
    0x8B, 0x00, 0x00, 0x00, 0x01, 0x01, 0x40 //129 - 135 (0x81 - 0x87)
};


//Reset sensor via software
void vl53l1x_softReset()
{
    addr16_writeRegister(devAddr, VL53L1_SOFT_RESET, 0x00); //Reset
    usleep(1000); //Driver uses 100us
    addr16_writeRegister(devAddr, VL53L1_SOFT_RESET, 0x01); //Exit reset
}

//Write the configuration block with a max of I2C_BUFFER_LENGTH bytes at a time
//Offset allows us to start at a location within the configBlock array
//This is the main function that setups up the VL53L1X to take measurements
//This was obtained by inspecting the example software from ST and by
//capturing I2C trace on ST Nucleo demo board
int vl53l1x_startMeasurement(uint8_t offset)
{
    int status, addrLength, dataLength;
    uint8_t address = 1 + offset; //Start at memory location 0x01, add offset
    addrLength = 2;
    
    uint8_t *addrBuffer = malloc(addrLength * sizeof(uint8_t));
    // split the address into bytes
    addrBuffer[1] = address;
    addrBuffer[0] = 0;
    
    dataLength = sizeof(configBlock);
    
    // perform the write 
    status = i2c_writeMultiByteAddr(OMEGA_I2C_DEV_NUM, devAddr, addrBuffer, addrLength, configBlock, dataLength);
    
    free(addrBuffer);
    return status;
    
    /*int status, length;
    uint8_t address = 1 + offset; //Start at memory location 0x01, add offset
    length = sizeof(configBlock);
    // printf(" > vl53l1x_startMeasurement about to send %d bytes of configBlock\n", length);
    status = i2c_writeBuffer(VL53L1X_I2C_DEV_NUM, devAddr, address, configBlock, length);
    printf(" > vl53l1x_startMeasurement write buffer of size %d starting at addr: 0x%02x\n", length, address);
    // for (int i = 0; i < length; i++) printf(" %d: 0x%02x\n", i, configBlock[i]);
    
    return status;*/
    /*
    uint8_t leftToSend = sizeof(configBlock) - offset;
    while (leftToSend > 0)
    {
        uint16_t toSend = I2C_BUFFER_LENGTH - 2; //Max I2C buffer on Arduino is 32, and we need 2 bytes for address
        if (toSend > leftToSend) toSend = leftToSend;
        
        _i2cPort->beginTransmission(_deviceAddress);
        
        _i2cPort->write(0); //We're only in lower address space. No MSB needed.
        _i2cPort->write(address);
        
        for (byte x = 0 ; x < toSend ; x++)
            _i2cPort->write(configBlock[address + x - 1 - offset]);
        
        _i2cPort->endTransmission();
        
        leftToSend -= toSend;
        address += toSend;
    }*/
}



///// API FUNCTIONS /////
//Check to see if sensor is responding
//Set sensor up for 2.8/3.3V I2C

int vl53l1x_setup()
{
    int status;
    devAddr = VL53L1X_I2C_DEV_ADDR;
    
    //Check the device ID
    uint16_t modelID = addr16_readRegister16(devAddr, VL53L1_IDENTIFICATION__MODEL_ID);
#if VL53L1X_DEBUG == 1
    printf("VL53L1X returned modelId: 0x%04x\n", modelID);
#endif // UTIL_DEBUG
    if (modelID != 0xEACC) return (EXIT_FAILURE);
    
    vl53l1x_softReset();
    
    //Polls the bit 0 of the FIRMWARE__SYSTEM_STATUS register to see if the firmware is ready
    int counter = 0;
    while (addr16_readRegister16(devAddr, VL53L1_FIRMWARE__SYSTEM_STATUS) & 0x01 == 0)
    {
        if (counter++ == 100) return EXIT_FAILURE; //Sensor timed out
        usleep(10*1000);
    }
    
    //Set I2C to 2.8V mode. In this mode 3.3V I2C is allowed.
    uint16_t result = addr16_readRegister16(devAddr, VL53L1_PAD_I2C_HV__EXTSUP_CONFIG);
    result = (result & 0xFE) | 0x01;
    addr16_writeRegister16(devAddr, VL53L1_PAD_I2C_HV__EXTSUP_CONFIG, result);
    
    //Gets trim resistors from chip
    for (uint16_t i = 0; i < 36; i++) {
        uint8_t regVal = addr16_readRegister(devAddr, i + 1);
        configBlock[i] = regVal;
    }
    status = vl53l1x_startMeasurement(0);
    
    return (status); //Sensor online!
}

//Polls the measurement completion bit
int vl53l1x_newDataReady()
{
    if (addr16_readRegister(devAddr, VL53L1_GPIO__TIO_HV_STATUS) != 0x03) return(1); //New measurement!
    return(0); //No new data
}

uint16_t vl53l1x_getDistance()
{
    return (addr16_readRegister16(devAddr, VL53L1_RESULT__FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD0));
}
