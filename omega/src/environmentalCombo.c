#include "bme280.h"
#include <stdio.h>
#include <stdlib.h>

int envComboSetup() {
    int status;
    
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
    float temp, humidity, pressure;
    
    temp = bme280_readTemperature();
    humidity = bme280_readHumidity();
    pressure = bme280_readPressure();
    
    printf("Temperature: %.02f C\n", temp);
    printf("Humidity:    %.02f %% RH\n", humidity);
    printf("pressure:    %.02f Pa\n", pressure);
    
    return EXIT_SUCCESS;
}