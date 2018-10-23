#include <stdlib.h>

#include <onion-i2c.h>
#include "ccs811.h"

int devAddr;

int ccs811_readRegister(uint8_t addr, uint8_t* data)
{
    int status;
    int tmp;
    status = i2c_readByte (CCS811_I2C_DEV_NUM, devAddr, addr, &tmp);
    (*data) = (uint8_t)tmp;
    return status;
}

int ccs811_writeRegister(uint8_t addr, uint8_t data)
{
    int status;
    status = i2c_write(CCS811_I2C_DEV_NUM, devAddr, addr, (uint8_t)data);
    return status;
}

// int multiReadRegister(uint8_t addr, uint8_t *buffer, uint8_t length) {
//     int status;
//     status = i2c_read(CCS811_I2C_DEV_NUM, devAddr, addr, buffer, length);
//     return status;
// }

int multiWriteRegister(uint8_t addr, uint8_t *buffer, uint8_t length) {
    int status;
    status = i2c_writeBuffer(CCS811_I2C_DEV_NUM, devAddr, addr, buffer, length);
    return status;
}

int ccs811_checkForStatusError( void )
{
	uint8_t value;
	//return the status bit
	ccs811_readRegister( CCS811_STATUS, &value );
	return (value & 1 << 0);
}

int ccs811_appValid( void )
{
	int status;
	uint8_t value;
	status = ccs811_readRegister( CCS811_STATUS, &value );
	if( status != EXIT_SUCCESS )
	{
		return 0;
	}
	else
	{
		return (value & 1 << 4);
	}
}

//Mode 0 = Idle
//Mode 1 = read every 1s
//Mode 2 = every 10s
//Mode 3 = every 60s
//Mode 4 = RAW mode
int ccs811_setDriveMode( uint8_t mode )
{
    int status;
    uint8_t value;
	if (mode > 4) mode = 4; //sanitize input

	status = ccs811_readRegister( CCS811_MEAS_MODE, &value ); //Read what's currently there
	if( status != EXIT_SUCCESS ) return status;
	
	value &= ~(0b00000111 << 4); //Clear DRIVE_MODE bits
	value |= (mode << 4); //Mask in mode
	status = ccs811_writeRegister(CCS811_MEAS_MODE, value);
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
    
    if( ccs811_checkForStatusError() == 1 ) {
        return EXIT_FAILURE;
    }
	if( ccs811_appValid() == 0 ) {
	    return EXIT_FAILURE;
	}
    
    // start the sensor
    uint8_t start[1] = {CCS811_APP_START};
    status |= i2c_writeBufferRaw	(CCS811_I2C_DEV_NUM, devAddr, start, 1);
    
    // allow sensor to start
    usleep(CCS811_SLEEP_MS * 1000);
    
    status |= ccs811_setDriveMode(1); //Read every second
    
    return status;
}

uint8_t ccs811_getErrorRegister( void )
{
    int status;
	uint8_t value;
	
	status = ccs811_readRegister( CCS811_ERROR_ID, &value );
	if( status != EXIT_SUCCESS )
	{
		return 0xFF;
	}
	else
	{
		return value;  //Send all errors in the event of communication error
	}
}

int ccs811_dataAvailable( void )
{
    int status;
	uint8_t value;
	status = ccs811_readRegister( CCS811_STATUS, &value );
	if( status != EXIT_SUCCESS )
	{
		return 0;
	}
	else
	{
		return (value & 1 << 3);
	}
}

int ccs811_readAlgorithmResults( uint16_t *CO2, uint16_t *tVOC )
{
	int status;
	int len = 4;
	uint8_t *data = malloc(len * sizeof(uint8_t));
#if CCS811_DEBUG == 1
    printf("> reading %d bytes from i2c-%d device 0x%02x, addr 0x%02x\n", len, CCS811_I2C_DEV_NUM, devAddr, CCS811_ALG_RESULT_DATA);
#endif // CCS811_DEBUG
    status = i2c_read(CCS811_I2C_DEV_NUM, devAddr, CCS811_ALG_RESULT_DATA, data, len);
	if( status != EXIT_SUCCESS ) return status;
#if CCS811_DEBUG == 1
	printf("read: 0x%02x, 0x%02x, 0x%02x, 0x%02x\n", data[3], data[2], data[1], data[0]);
#endif // CCS811_DEBUG
	// Data ordered:
	// 0       1       2        3 
	// co2MSB, co2LSB, tvocMSB, tvocLSB
	
	(*CO2) = ((uint16_t)data[0] << 8) | data[1];
	(*tVOC) = ((uint16_t)data[2] << 8) | data[3];
	free(data);
	return EXIT_SUCCESS;
}

//Given a temp and humidity, write this data to the device for better compensation
int ccs811_setEnvironmentalData( float relativeHumidity, float temperature )
{
    int status; 
    uint8_t envData[4];
    
	//Check for invalid temperatures
	if((temperature < -25)||(temperature > 50)) return EXIT_FAILURE;
	
	//Check for invalid humidity
	if((relativeHumidity < 0)||(relativeHumidity > 100)) return EXIT_FAILURE;
	
	uint32_t rH = relativeHumidity * 1000; //42.348 becomes 42348
	uint32_t temp = temperature * 1000; //23.2 becomes 23200

	//Split value into 7-bit integer and 9-bit fractional
	envData[0] = (rH + 250) / 500;
	envData[1] = 0; //CCS811 only supports increments of 0.5 so bits 7-0 will always be zero

	temp += 25000; //Add the 25C offset

	envData[2] = (temp + 250) / 500;
	envData[3] = 0;
	
	status = multiWriteRegister(CCS811_ENV_DATA, envData, 4);
	return status;
}

uint8_t ccs811_checkStatus( void )
{
	uint8_t value;
	//return the status bit
	ccs811_readRegister( CCS811_STATUS, &value );
	return (value);
}