#include <stdio.h>
#include <stdlib.h>

#include "bme280.h"
#include "ccs811.h"

int envComboSetup() {
    int status;
    
    status = ccs811_setup();
    if (status != EXIT_SUCCESS) {
        printf("ERROR: could not initialize CCS811 device\n");
    }
    printf("CCS811 status is 0x%02x\n", ccs811_checkStatus());
    
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

int envComboRead() {
    int status;
    float temp, humidity, pressure;
    uint16_t CO2, tVOC;
    
    while (1) {
        temp = bme280_readTemperature();
        humidity = bme280_readHumidity();
        pressure = bme280_readPressure();
        
        // write temperature and humidity data to CCS811
        ccs811_setEnvironmentalData(humidity, temp);
        printf("CCS811 status is 0x%02x\n", ccs811_checkStatus());
        
        sleep(2);
        printf("Temperature: %.02f C\n", temp);
        printf("Humidity:    %.02f %% RH\n", humidity);
        printf("Pressure:    %.02f Pa\n", pressure);
        printf("CCS811 status is 0x%02x\n", ccs811_checkStatus());
        if (ccs811_dataAvailable())
        {
            status = ccs811_readAlgorithmResults(&CO2, &tVOC);
            if (status != EXIT_SUCCESS) {
                printf("ERROR: reading data from CCS811\n");
            } else {
                printf("CO2:    %d ppm\n", CO2);
                printf("tVOC:   %d ppb\n", tVOC);
                printf("CCS811 status is 0x%02x\n", ccs811_checkStatus());
            }
        } else {
            printf("CCS811 sensor has no data\n");
            printf("  error code: 0x%02x\n", ccs811_getErrorRegister());
        }
    }
    
    return status;
}

