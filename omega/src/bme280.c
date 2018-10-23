#include <stdlib.h>

#include <onion-i2c.h>
#include "bme280.h"

// global variables
struct SensorSettings settings;
struct SensorCalibration calibration;

uint8_t readRegister(uint8_t addr)
{
    int status;
    int val;
    status = i2c_readByte (BME280_I2C_DEV_NUM, settings.I2CAddress, addr, &val);
    return (uint8_t)val;
}

void writeRegister(uint8_t addr, uint8_t data)
{
    int status;
    status = i2c_write(BME280_I2C_DEV_NUM, settings.I2CAddress, addr, (uint8_t)data);
}


// Set the mode bits
void setMode(uint8_t mode)
{
	if(mode > 0b11) mode = 0; //Error check. Default to sleep mode
	
	uint8_t controlData = readRegister(BME280_CTRL_MEAS_REG);
	controlData &= ~( (1<<1) | (1<<0) ); //Clear the mode[1:0] bits
	controlData |= mode; //Set
	writeRegister(BME280_CTRL_MEAS_REG, controlData);
}

// Get the current mode bits
uint8_t getMode()
{
	uint8_t controlData = readRegister(BME280_CTRL_MEAS_REG);
	return(controlData & 0b00000011); //return bits 1 & 0
}

void setStandbyTime(uint8_t timeSetting)
{
	if(timeSetting > 0b111) timeSetting = 0; //Error check. Default to 0.5ms
	
	uint8_t controlData = readRegister(BME280_CONFIG_REG);
	controlData &= ~( (1<<7) | (1<<6) | (1<<5) ); //Clear the 7/6/5 bits
	controlData |= (timeSetting << 5); //Align with bits 7/6/5
	writeRegister(BME280_CONFIG_REG, controlData);
}

void setFilter(uint8_t filterSetting)
{
	if(filterSetting > 0b111) filterSetting = 0; //Error check. Default to filter off
	
	uint8_t controlData = readRegister(BME280_CONFIG_REG);
	controlData &= ~( (1<<4) | (1<<3) | (1<<2) ); //Clear the 4/3/2 bits
	controlData |= (filterSetting << 2); //Align with bits 4/3/2
	writeRegister(BME280_CONFIG_REG, controlData);
}

// validate over-sample value: allowed values are 0 to 16
uint8_t checkSampleValue(uint8_t userValue)
{
	switch(userValue) 
	{
		case(0): 
			return 0;
			break; //Valid
		case(1): 
			return 1;
			break; //Valid
		case(2): 
			return 2;
			break; //Valid
		case(4): 
			return 3;
			break; //Valid
		case(8): 
			return 4;
			break; //Valid
		case(16): 
			return 5;
			break; //Valid
		default: 
			return 1; //Default to 1x
			break; //Good
	}
}

void setTempOverSample(uint8_t overSampleAmount)
{
	overSampleAmount = checkSampleValue(overSampleAmount); //Error check
	
	uint8_t originalMode = getMode(); //Get the current mode so we can go back to it at the end
	
	setMode(BME280_MODE_SLEEP); //Config will only be writeable in sleep mode, so first go to sleep mode

	//Set the osrs_t bits (7, 6, 5) to overSampleAmount
	uint8_t controlData = readRegister(BME280_CTRL_MEAS_REG);
	controlData &= ~( (1<<7) | (1<<6) | (1<<5) ); //Clear bits 765
	controlData |= overSampleAmount << 5; //Align overSampleAmount to bits 7/6/5
	writeRegister(BME280_CTRL_MEAS_REG, controlData);
	
	setMode(originalMode); //Return to the original user's choice
}

void setPressureOverSample(uint8_t overSampleAmount)
{
	overSampleAmount = checkSampleValue(overSampleAmount); //Error check
	
	uint8_t originalMode = getMode(); //Get the current mode so we can go back to it at the end
	
	setMode(BME280_MODE_SLEEP); //Config will only be writeable in sleep mode, so first go to sleep mode

	//Set the osrs_p bits (4, 3, 2) to overSampleAmount
	uint8_t controlData = readRegister(BME280_CTRL_MEAS_REG);
	controlData &= ~( (1<<4) | (1<<3) | (1<<2) ); //Clear bits 432
	controlData |= overSampleAmount << 2; //Align overSampleAmount to bits 4/3/2
	writeRegister(BME280_CTRL_MEAS_REG, controlData);
	
	setMode(originalMode); //Return to the original user's choice
}

void setHumidityOverSample(uint8_t overSampleAmount)
{
	overSampleAmount = checkSampleValue(overSampleAmount); //Error check
	
	uint8_t originalMode = getMode(); //Get the current mode so we can go back to it at the end
	
	setMode(BME280_MODE_SLEEP); //Config will only be writeable in sleep mode, so first go to sleep mode

	//Set the osrs_h bits (2, 1, 0) to overSampleAmount
	uint8_t controlData = readRegister(BME280_CTRL_HUMIDITY_REG);
	controlData &= ~( (1<<2) | (1<<1) | (1<<0) ); //Clear bits 2/1/0
	controlData |= overSampleAmount << 0; //Align overSampleAmount to bits 2/1/0
	writeRegister(BME280_CTRL_HUMIDITY_REG, controlData);

	setMode(originalMode); //Return to the original user's choice
}

///// API FUNCTIONS /////
int bme280_setup(int devAddr, int runMode, int tStandby, int filter, int tempOverSample, int pressOverSample, int humidOverSample) {
    uint8_t status;
    
    // initialize settings
    settings.I2CAddress = devAddr;
	settings.runMode = runMode; // default should be 3 - Normal/Run
	settings.tStandby = tStandby; // default: 0 - 0.5ms
	settings.filter = filter; // default: 0 - Filter off
	settings.tempOverSample = tempOverSample; // default: 1
	settings.pressOverSample = pressOverSample; // default: 1
	settings.humidOverSample = humidOverSample; // default: 1
	
    // allow sensor to turn on
    usleep(BME280_SLEEP_MS * 1000);
    
    // read calibration data
	calibration.dig_T1 = ((uint16_t)((readRegister(BME280_DIG_T1_MSB_REG) << 8) + readRegister(BME280_DIG_T1_LSB_REG)));
	calibration.dig_T2 = ((int16_t)((readRegister(BME280_DIG_T2_MSB_REG) << 8) + readRegister(BME280_DIG_T2_LSB_REG)));
	calibration.dig_T3 = ((int16_t)((readRegister(BME280_DIG_T3_MSB_REG) << 8) + readRegister(BME280_DIG_T3_LSB_REG)));

	calibration.dig_P1 = ((uint16_t)((readRegister(BME280_DIG_P1_MSB_REG) << 8) + readRegister(BME280_DIG_P1_LSB_REG)));
	calibration.dig_P2 = ((int16_t)((readRegister(BME280_DIG_P2_MSB_REG) << 8) + readRegister(BME280_DIG_P2_LSB_REG)));
	calibration.dig_P3 = ((int16_t)((readRegister(BME280_DIG_P3_MSB_REG) << 8) + readRegister(BME280_DIG_P3_LSB_REG)));
	calibration.dig_P4 = ((int16_t)((readRegister(BME280_DIG_P4_MSB_REG) << 8) + readRegister(BME280_DIG_P4_LSB_REG)));
	calibration.dig_P5 = ((int16_t)((readRegister(BME280_DIG_P5_MSB_REG) << 8) + readRegister(BME280_DIG_P5_LSB_REG)));
	calibration.dig_P6 = ((int16_t)((readRegister(BME280_DIG_P6_MSB_REG) << 8) + readRegister(BME280_DIG_P6_LSB_REG)));
	calibration.dig_P7 = ((int16_t)((readRegister(BME280_DIG_P7_MSB_REG) << 8) + readRegister(BME280_DIG_P7_LSB_REG)));
	calibration.dig_P8 = ((int16_t)((readRegister(BME280_DIG_P8_MSB_REG) << 8) + readRegister(BME280_DIG_P8_LSB_REG)));
	calibration.dig_P9 = ((int16_t)((readRegister(BME280_DIG_P9_MSB_REG) << 8) + readRegister(BME280_DIG_P9_LSB_REG)));

	calibration.dig_H1 = ((uint8_t)(readRegister(BME280_DIG_H1_REG)));
	calibration.dig_H2 = ((int16_t)((readRegister(BME280_DIG_H2_MSB_REG) << 8) + readRegister(BME280_DIG_H2_LSB_REG)));
	calibration.dig_H3 = ((uint8_t)(readRegister(BME280_DIG_H3_REG)));
	calibration.dig_H4 = ((int16_t)((readRegister(BME280_DIG_H4_MSB_REG) << 4) + (readRegister(BME280_DIG_H4_LSB_REG) & 0x0F)));
	calibration.dig_H5 = ((int16_t)((readRegister(BME280_DIG_H5_MSB_REG) << 4) + ((readRegister(BME280_DIG_H4_LSB_REG) >> 4) & 0x0F)));
	calibration.dig_H6 = ((int8_t)readRegister(BME280_DIG_H6_REG));
	
	// initialize the sensor
	setStandbyTime(settings.tStandby);
	setFilter(settings.filter);
	setPressureOverSample(settings.pressOverSample);
	setHumidityOverSample(settings.humidOverSample);
	setTempOverSample(settings.tempOverSample);
	
	// start the sensor
	setMode(BME280_MODE_NORMAL);
	
	// check for sensor ok
	status = readRegister(BME280_CHIP_ID_REG);
	if (status == 0x60) {
	    return EXIT_SUCCESS;
	} else {
	    return EXIT_FAILURE;
	}
}
