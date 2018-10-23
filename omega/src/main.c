#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include "messageQueue.h"
#include "amg8833.h"
#include "environmentalCombo.h"
#include "vl53l1x.h"


void printUsage(char* progName) {
	printf("Usage: %s <DEVICE>\n", progName);
	printf("\nAvailable devices:\n\t%s\n\t%s\n\t%s\n\n", AMG8833_DEV_NAME, ENV_COMBO_DEV_NAME, VL53L1X_DEV_NAME);
}

int amg8833_device() {
	int status;
	float *pixelData = malloc(AMG8833_NUM_PIXELS * sizeof(float));
	char *msgData = malloc(512 * sizeof(char));
	
	status = amg8833_setup();

	// infinite loop
	while (status == EXIT_SUCCESS) {
		status = amg8833_readPixels(pixelData);
		amg8833_generateJsonArray(pixelData, msgData);
		// printf("msg: '%s'\n", msgData);
		sendMessage("/console/qwiic-amg8833", msgData);
		
		usleep(AMG8833_SLEEP_MS * 1000);
	}
	
	free(pixelData);
	free(msgData);
	return status;
}

int envComboDevice() {
	int status;
	float temp, humidity, pressure;
	uint16_t CO2, tVOC;
	char *msgData = malloc(512 * sizeof(char));
	
	status = envComboSetup();

	// infinite loop
	while (status == EXIT_SUCCESS) {
		envComboRead(&temp, &humidity, &pressure, &CO2, &tVOC);
		envComboGenerateJson(msgData, temp, humidity, pressure, CO2, tVOC);
		// printf("msg: '%s'\n", msgData);
		sendMessage("/console/qwiic-env", msgData);
		
		usleep(ENV_COMBO_SLEEP_MS * 1000);
	}
	
	free(msgData);
	return status;
}

int vl53l1x_device() {
	int status;
	uint16_t data;
	char *msgData = malloc(512 * sizeof(char));
	
	status = vl53l1x_setup();
	// printf("vl53l1x_setup returned %d\n", status);

	// infinite loop
	while (status == EXIT_SUCCESS) {
		if (vl53l1x_newDataReady()) {
			data = vl53l1x_getDistance();
			// printf("distance: %d\n", data);
			sprintf(msgData, "%d", data);
			// printf("msg: '%s'\n", msgData);
			sendMessage("/console/qwiic-vl53l1x", msgData);
		}
		
		usleep(VL53L1X_SLEEP_MS * 1000);
	}
	
	free(msgData);
	return status;
}


int main(int argc, char *argv[]) {
	int status = 0;
	printf("Program version: %s %s\n", __DATE__, __TIME__);
	
	// check for arguments
	if (argc < 2) {
		printUsage(argv[0]);
		return 1;
	}
	
	// initialize message queue
	status = initMessageQueue(MQ_DEFAULT_HOST, MQ_DEFAULT_PORT);
	if (status != EXIT_SUCCESS) {
		printf("ERROR: Unable to connect to MQTT broker\n");
		return 1;
	}
	waitMessageQueueConnected();
	
	// check which device is to be used
	if (strcmp(argv[1], AMG8833_DEV_NAME) == 0) {
		status = amg8833_device();	
	}
	else if (strcmp(argv[1], ENV_COMBO_DEV_NAME) == 0) {
		status = envComboDevice();	
	}
	else if (strcmp(argv[1], VL53L1X_DEV_NAME) == 0) {
		status = vl53l1x_device();	
	}
	else {
		printf("ERROR: Unknown device!\n");
		printUsage(argv[0]);
	}

	destroyMessageQueue();
	return 0;
}
