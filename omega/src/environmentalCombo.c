#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "bme280.h"
#include "ccs811.h"

int envComboSetup() {
    int status;
    
    status = ccs811_setup();
    if (status != EXIT_SUCCESS) {
        printf("ERROR: could not initialize CCS811 device\n");
    }
    // printf("CCS811 status is 0x%02x\n", ccs811_checkStatus());
    
    status = bme280_setup(   
                BME280_I2C_DEV_ADDR,    // I2C device addr
                BME280_MODE_NORMAL,     // runMode
                0,      // tStandby
                4,      // filter
                5,      // tempOverSample
                5,      // pressOverSample
                5       // humidOverSample
            );
	
    return status;
}

int envComboRead(float *temp, float *humidity, float *pressure, uint16_t *CO2, uint16_t *tVOC) {
    int status = EXIT_SUCCESS;
    int count = 0;
    int maxCount = 5;
    
    // read BME280 sensor
    (*temp) = bme280_readTemperature();
    (*humidity) = bme280_readHumidity();
    (*pressure) = bme280_readPressure();
    // convert pressure from Pa to kPa
    (*pressure) = (*pressure) / 1000.0;
    // write temperature and humidity data to CCS811
    ccs811_setEnvironmentalData((*humidity), (*temp));
    
    // read from CCS811
    while (count < maxCount) {
        if (ccs811_dataAvailable())
        {
            status = ccs811_readAlgorithmResults(CO2, tVOC);
            if (status != EXIT_SUCCESS) {
                printf("ERROR: reading data from CCS811\n");
            } 
            break;
        } 
        count++;
        usleep(50 * 1000);
    }
    if (count >= maxCount) {
        // printf("CCS811 sensor has no data\n");
        // printf("  error code: 0x%02x\n", ccs811_getErrorRegister());
        (*CO2) = 0;
        (*tVOC) = 0;
    }
    
    return status;
}

void envComboGenerateJson(char *json, float temp, float humidity, float pressure, uint16_t CO2, uint16_t tVOC, const char* identifier) {
    sprintf(json, "{\"identifier\":%s,\"temperature\":%f, \"humidity\":%f, \"pressure\":%f, \"co2\":%d, \"tvoc\":%d}", identifier, temp, humidity, pressure, CO2, tVOC);
}
