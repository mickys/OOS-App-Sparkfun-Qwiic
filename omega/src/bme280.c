#include <stdlib.h>

#include <onion-i2c.h>
#include "util.h"
#include "bme280.h"

// global variables
struct SensorSettings settings;
struct SensorCalibration calibration;
int32_t t_fine;

int readRegisterRegion(uint8_t *rdBuffer , uint8_t addr, uint8_t length)
{
    int status;
    status = i2c_read(BME280_I2C_DEV_NUM, settings.I2CAddress, addr, rdBuffer, length);  
    return status;
}

// Set the mode bits
void setMode(uint8_t mode)
{
	if(mode > 0b11) mode = 0; //Error check. Default to sleep mode
	
	uint8_t controlData = readRegisterReturn(settings.I2CAddress, BME280_CTRL_MEAS_REG);
	controlData &= ~( (1<<1) | (1<<0) ); //Clear the mode[1:0] bits
	controlData |= mode; //Set
	writeRegister(settings.I2CAddress, BME280_CTRL_MEAS_REG, controlData);
}

// Get the current mode bits
uint8_t getMode()
{
	uint8_t controlData = readRegisterReturn(settings.I2CAddress, BME280_CTRL_MEAS_REG);
	return(controlData & 0b00000011); //return bits 1 & 0
}

void setStandbyTime(uint8_t timeSetting)
{
	if(timeSetting > 0b111) timeSetting = 0; //Error check. Default to 0.5ms
	
	uint8_t controlData = readRegisterReturn(settings.I2CAddress, BME280_CONFIG_REG);
	controlData &= ~( (1<<7) | (1<<6) | (1<<5) ); //Clear the 7/6/5 bits
	controlData |= (timeSetting << 5); //Align with bits 7/6/5
	writeRegister(settings.I2CAddress, BME280_CONFIG_REG, controlData);
}

void setFilter(uint8_t filterSetting)
{
	if(filterSetting > 0b111) filterSetting = 0; //Error check. Default to filter off
	
	uint8_t controlData = readRegisterReturn(settings.I2CAddress, BME280_CONFIG_REG);
	controlData &= ~( (1<<4) | (1<<3) | (1<<2) ); //Clear the 4/3/2 bits
	controlData |= (filterSetting << 2); //Align with bits 4/3/2
	writeRegister(settings.I2CAddress, BME280_CONFIG_REG, controlData);
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
	uint8_t controlData = readRegisterReturn(settings.I2CAddress, BME280_CTRL_MEAS_REG);
	controlData &= ~( (1<<7) | (1<<6) | (1<<5) ); //Clear bits 765
	controlData |= overSampleAmount << 5; //Align overSampleAmount to bits 7/6/5
	writeRegister(settings.I2CAddress, BME280_CTRL_MEAS_REG, controlData);
	
	setMode(originalMode); //Return to the original user's choice
}

void setPressureOverSample(uint8_t overSampleAmount)
{
	overSampleAmount = checkSampleValue(overSampleAmount); //Error check
	
	uint8_t originalMode = getMode(); //Get the current mode so we can go back to it at the end
	
	setMode(BME280_MODE_SLEEP); //Config will only be writeable in sleep mode, so first go to sleep mode

	//Set the osrs_p bits (4, 3, 2) to overSampleAmount
	uint8_t controlData = readRegisterReturn(settings.I2CAddress, BME280_CTRL_MEAS_REG);
	controlData &= ~( (1<<4) | (1<<3) | (1<<2) ); //Clear bits 432
	controlData |= overSampleAmount << 2; //Align overSampleAmount to bits 4/3/2
	writeRegister(settings.I2CAddress, BME280_CTRL_MEAS_REG, controlData);
	
	setMode(originalMode); //Return to the original user's choice
}

void setHumidityOverSample(uint8_t overSampleAmount)
{
	overSampleAmount = checkSampleValue(overSampleAmount); //Error check
	
	uint8_t originalMode = getMode(); //Get the current mode so we can go back to it at the end
	
	setMode(BME280_MODE_SLEEP); //Config will only be writeable in sleep mode, so first go to sleep mode

	//Set the osrs_h bits (2, 1, 0) to overSampleAmount
	uint8_t controlData = readRegisterReturn(settings.I2CAddress, BME280_CTRL_HUMIDITY_REG);
	controlData &= ~( (1<<2) | (1<<1) | (1<<0) ); //Clear bits 2/1/0
	controlData |= overSampleAmount << 0; //Align overSampleAmount to bits 2/1/0
	writeRegister(settings.I2CAddress, BME280_CTRL_HUMIDITY_REG, controlData);

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
	
	t_fine = (int32_t)0;
	
    // allow sensor to turn on
    usleep(BME280_SLEEP_MS * 1000);
    
    // read calibration data
	calibration.dig_T1 = ((uint16_t)((readRegisterReturn(settings.I2CAddress, BME280_DIG_T1_MSB_REG) << 8) + readRegisterReturn(settings.I2CAddress, BME280_DIG_T1_LSB_REG)));
	calibration.dig_T2 = ((int16_t)((readRegisterReturn(settings.I2CAddress, BME280_DIG_T2_MSB_REG) << 8) + readRegisterReturn(settings.I2CAddress, BME280_DIG_T2_LSB_REG)));
	calibration.dig_T3 = ((int16_t)((readRegisterReturn(settings.I2CAddress, BME280_DIG_T3_MSB_REG) << 8) + readRegisterReturn(settings.I2CAddress, BME280_DIG_T3_LSB_REG)));

	calibration.dig_P1 = ((uint16_t)((readRegisterReturn(settings.I2CAddress, BME280_DIG_P1_MSB_REG) << 8) + readRegisterReturn(settings.I2CAddress, BME280_DIG_P1_LSB_REG)));
	calibration.dig_P2 = ((int16_t)((readRegisterReturn(settings.I2CAddress, BME280_DIG_P2_MSB_REG) << 8) + readRegisterReturn(settings.I2CAddress, BME280_DIG_P2_LSB_REG)));
	calibration.dig_P3 = ((int16_t)((readRegisterReturn(settings.I2CAddress, BME280_DIG_P3_MSB_REG) << 8) + readRegisterReturn(settings.I2CAddress, BME280_DIG_P3_LSB_REG)));
	calibration.dig_P4 = ((int16_t)((readRegisterReturn(settings.I2CAddress, BME280_DIG_P4_MSB_REG) << 8) + readRegisterReturn(settings.I2CAddress, BME280_DIG_P4_LSB_REG)));
	calibration.dig_P5 = ((int16_t)((readRegisterReturn(settings.I2CAddress, BME280_DIG_P5_MSB_REG) << 8) + readRegisterReturn(settings.I2CAddress, BME280_DIG_P5_LSB_REG)));
	calibration.dig_P6 = ((int16_t)((readRegisterReturn(settings.I2CAddress, BME280_DIG_P6_MSB_REG) << 8) + readRegisterReturn(settings.I2CAddress, BME280_DIG_P6_LSB_REG)));
	calibration.dig_P7 = ((int16_t)((readRegisterReturn(settings.I2CAddress, BME280_DIG_P7_MSB_REG) << 8) + readRegisterReturn(settings.I2CAddress, BME280_DIG_P7_LSB_REG)));
	calibration.dig_P8 = ((int16_t)((readRegisterReturn(settings.I2CAddress, BME280_DIG_P8_MSB_REG) << 8) + readRegisterReturn(settings.I2CAddress, BME280_DIG_P8_LSB_REG)));
	calibration.dig_P9 = ((int16_t)((readRegisterReturn(settings.I2CAddress, BME280_DIG_P9_MSB_REG) << 8) + readRegisterReturn(settings.I2CAddress, BME280_DIG_P9_LSB_REG)));

	calibration.dig_H1 = ((uint8_t)(readRegisterReturn(settings.I2CAddress, BME280_DIG_H1_REG)));
	calibration.dig_H2 = ((int16_t)((readRegisterReturn(settings.I2CAddress, BME280_DIG_H2_MSB_REG) << 8) + readRegisterReturn(settings.I2CAddress, BME280_DIG_H2_LSB_REG)));
	calibration.dig_H3 = ((uint8_t)(readRegisterReturn(settings.I2CAddress, BME280_DIG_H3_REG)));
	calibration.dig_H4 = ((int16_t)((readRegisterReturn(settings.I2CAddress, BME280_DIG_H4_MSB_REG) << 4) + (readRegisterReturn(settings.I2CAddress, BME280_DIG_H4_LSB_REG) & 0x0F)));
	calibration.dig_H5 = ((int16_t)((readRegisterReturn(settings.I2CAddress, BME280_DIG_H5_MSB_REG) << 4) + ((readRegisterReturn(settings.I2CAddress, BME280_DIG_H4_LSB_REG) >> 4) & 0x0F)));
	calibration.dig_H6 = ((int8_t)readRegisterReturn(settings.I2CAddress, BME280_DIG_H6_REG));
	
	// initialize the sensor
	setStandbyTime(settings.tStandby);
	setFilter(settings.filter);
	setPressureOverSample(settings.pressOverSample);
	setHumidityOverSample(settings.humidOverSample);
	setTempOverSample(settings.tempOverSample);
	
	// start the sensor
	setMode(BME280_MODE_NORMAL);
	
	// check for sensor ok
	status = readRegisterReturn(settings.I2CAddress, BME280_CHIP_ID_REG);
	if (status == 0x60) {
	    return EXIT_SUCCESS;
	} else {
	    return EXIT_FAILURE;
	}
}

float bme280_readTemperature( void )
{
    int status;
    // Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
    // t_fine carries fine temperature as global value
    
    //get the reading (adc_T);
    uint8_t *buffer = malloc(3 * sizeof(uint8_t));
    status = readRegisterRegion(buffer, BME280_TEMPERATURE_MSB_REG, 3);
    // onion.io: assuming this type of byte alignment when reading: 2,1,0
    // int32_t adc_T = ((uint32_t)buffer[2] << 12) | ((uint32_t)buffer[1] << 4) | ((buffer[0] >> 4) & 0x0F);
    int32_t adc_T = ((uint32_t)buffer[0] << 12) | ((uint32_t)buffer[1] << 4) | ((buffer[2] >> 4) & 0x0F);
#if BME280_DEBUG == 1
    printf("temp read: 0x%02x, 0x%02x, 0x%02x; adc_T = 0x%08x\n", buffer[0], buffer[1], buffer[2], adc_T);
#endif // BME280_DEBUG

    //By datasheet, calibrate
    int64_t var1, var2;
    
    var1 = ((((adc_T>>3) - ((int32_t)calibration.dig_T1<<1))) * ((int32_t)calibration.dig_T2)) >> 11;
    var2 = (((((adc_T>>4) - ((int32_t)calibration.dig_T1)) * ((adc_T>>4) - ((int32_t)calibration.dig_T1))) >> 12) *
    ((int32_t)calibration.dig_T3)) >> 14;
    t_fine = var1 + var2;
    float output = (t_fine * 5 + 128) >> 8;
    
    output = output / 100;
    
    free(buffer);
    return output;
}

float bme280_readHumidity( void )
{
    // Returns humidity in %RH as unsigned 32 bit integer in Q22. 10 format (22 integer and 10 fractional bits).
    // Output value of “47445” represents 47445/1024 = 46. 333 %RH
    uint8_t *buffer = malloc(2 * sizeof(uint8_t));
    readRegisterRegion(buffer, BME280_HUMIDITY_MSB_REG, 2);
    // onion.io: assuming this type of byte alignment when reading: 1,0
    // int32_t adc_H = ((uint32_t)buffer[1] << 8) | ((uint32_t)buffer[0]);
    int32_t adc_H = ((uint32_t)buffer[0] << 8) | ((uint32_t)buffer[1]);
    
#if BME280_DEBUG == 1
    printf("humidity read: 0x%02x, 0x%02x; adc_H = 0x%08x\n", buffer[0], buffer[1], adc_H);
#endif // BME280_DEBUG
    
    int32_t var1;
    var1 = (t_fine - ((int32_t)76800));
    var1 = (((((adc_H << 14) - (((int32_t)calibration.dig_H4) << 20) - (((int32_t)calibration.dig_H5) * var1)) +
    ((int32_t)16384)) >> 15) * (((((((var1 * ((int32_t)calibration.dig_H6)) >> 10) * (((var1 * ((int32_t)calibration.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) *
    ((int32_t)calibration.dig_H2) + 8192) >> 14));
    var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) * ((int32_t)calibration.dig_H1)) >> 4));
    var1 = (var1 < 0 ? 0 : var1);
    var1 = (var1 > 419430400 ? 419430400 : var1);
    
    free(buffer);
    return (float)(var1>>12) / 1024.0;
}

float bme280_readPressure( void )
{

	// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
	// Output value of “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
    uint8_t *buffer = malloc(3 * sizeof(uint8_t));
	readRegisterRegion(buffer, BME280_PRESSURE_MSB_REG, 3);
	// onion.io: assuming this type of byte alignment when reading: 2,1,0
// 	int32_t adc_P = ((uint32_t)buffer[2] << 12) | ((uint32_t)buffer[1] << 4) | ((buffer[0] >> 4) & 0x0F);
    int32_t adc_P = ((uint32_t)buffer[0] << 12) | ((uint32_t)buffer[1] << 4) | ((buffer[2] >> 4) & 0x0F);
    
#if BME280_DEBUG == 1
    printf("pressure read: 0x%02x, 0x%02x, 0x%02x; adc_P = 0x%08x\n", buffer[0], buffer[1], buffer[2], adc_P);
#endif // BME280_DEBUG
	
	int64_t var1, var2, p_acc;
	var1 = ((int64_t)t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)calibration.dig_P6;
	var2 = var2 + ((var1 * (int64_t)calibration.dig_P5)<<17);
	var2 = var2 + (((int64_t)calibration.dig_P4)<<35);
	var1 = ((var1 * var1 * (int64_t)calibration.dig_P3)>>8) + ((var1 * (int64_t)calibration.dig_P2)<<12);
	var1 = (((((int64_t)1)<<47)+var1))*((int64_t)calibration.dig_P1)>>33;
	if (var1 == 0)
	{
		return 0; // avoid exception caused by division by zero
	}
	p_acc = 1048576 - adc_P;
	p_acc = (((p_acc<<31) - var2)*3125)/var1;
	var1 = (((int64_t)calibration.dig_P9) * (p_acc>>13) * (p_acc>>13)) >> 25;
	var2 = (((int64_t)calibration.dig_P8) * p_acc) >> 19;
	p_acc = ((p_acc + var1 + var2) >> 8) + (((int64_t)calibration.dig_P7)<<4);
	
	free(buffer);
	return (float)p_acc / 256.0;
}

