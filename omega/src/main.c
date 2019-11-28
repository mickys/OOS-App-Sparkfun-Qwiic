#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <libconfig.h>
#include <limits.h>

#include "messageQueue.h"
#include "amg8833.h"
#include "environmentalCombo.h"
#include "vl53l1x.h"


void printUsage(char* progName) {
	printf("Usage: %s <DEVICE>\n", progName);
	printf("\nAvailable devices:\n\t%s\n\t%s\n\t%s\n\n", AMG8833_DEV_NAME, ENV_COMBO_DEV_NAME, VL53L1X_DEV_NAME);
}

int amg8833_device(const char* identifier, const char* topic) {
	int status;
	float *pixelData = malloc(AMG8833_NUM_PIXELS * sizeof(float));
	char *msgData = malloc(512 * sizeof(char));
	
	status = amg8833_setup();

	// infinite loop
	while (status == EXIT_SUCCESS) {
		status = amg8833_readPixels(pixelData);
		amg8833_generateJsonArray(pixelData, msgData, identifier);
		// printf("msg: '%s'\n", msgData);
		sendMessage(topic, msgData);
		
		usleep(AMG8833_SLEEP_MS * 1000);
	}
	
	free(pixelData);
	free(msgData);
	return status;
}

int envComboDevice(const char* identifier, const char* topic, int time, const char* ds18cmd) {
	int status;
	float temp, humidity, pressure, ds18b20;
	uint16_t CO2, tVOC;
	char *msgData = malloc(512 * sizeof(char));
	
	status = envComboSetup();

	// infinite loop
	while (status == EXIT_SUCCESS) {
		envComboRead(&temp, &humidity, &pressure, &CO2, &tVOC);
		envReadDS18(ds18cmd, &ds18b20);
		envComboGenerateJson(msgData, temp, humidity, pressure, CO2, tVOC, identifier, ds18b20);
		// printf("msg: '%s'\n", msgData);
		sendMessage(topic, msgData);
		
		usleep(ENV_COMBO_SLEEP_MS * time);
	}
	
	free(msgData);
	return status;
}

int vl53l1x_device(const char* identifier, const char* topic) {
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
			sendMessage(topic, msgData);
		}
		
		usleep(VL53L1X_SLEEP_MS * 1000);
	}
	
	free(msgData);
	return status;
}


int main(int argc, char *argv[]) {
	int status = 0;
	// printf("Program version: %s %s\n", __DATE__, __TIME__);

	// check for arguments
	if (argc < 2) {
		printUsage(argv[0]);
		return 1;
	}

	config_t cfg;
	config_setting_t *setting;
	const char* server, identifier, topic, certificate, ds18cmd;
	int port, time;

	config_init(&cfg);
	/* Read the file. If there is an error, report it and exit. */
	if(! config_read_file(&cfg, "/etc/env-reporter/config.cfg"))
	{
		fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
				config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);
		return(EXIT_FAILURE);
	}

	/* Get the identifier. */
	if(!config_lookup_string(&cfg, "id", &identifier)) {
		fprintf(stderr, "No 'id' setting in configuration file.\n");
		return(EXIT_FAILURE);
	} else {
		printf("ID: %s\n", identifier);
	}

	/* Get the topic. */
	if(!config_lookup_string(&cfg, "topic", &topic)) {
		fprintf(stderr, "No 'topic' setting in configuration file.\n");
		return(EXIT_FAILURE);
	} else {
		printf("topic: %s\n", topic);
	}

	/* Get the server name. */
	if(!config_lookup_string(&cfg, "server", &server)) {
		fprintf(stderr, "No 'server' setting in configuration file.\n");
		return(EXIT_FAILURE);
	} else {
		printf("Server name: %s\n", server);
	}

	/* Get the port. */
	if(!config_lookup_int(&cfg, "port", &port)) {
		fprintf(stderr, "No 'port' setting in configuration file.\n");
		return(EXIT_FAILURE);
	} else {
		printf("port: %d\n", port);
	}

	/* Get the time delay. */
	if(!config_lookup_int(&cfg, "time", &time)) {
		fprintf(stderr, "No 'time' setting in configuration file.\n");
		return(EXIT_FAILURE);
	} else {
		printf("Loop time: %d\n", time);
	}

	/* Get the certificate file. */
	if(!config_lookup_string(&cfg, "certificate", &certificate)) {
		fprintf(stderr, "No 'certificate' setting in configuration file.\n");
		return(EXIT_FAILURE);
	} else {
		printf("certificate: %s\n", certificate);
	}

	/* Get the ds18cmd file. */
	if(!config_lookup_string(&cfg, "ds18cmd", &ds18cmd)) {
		fprintf(stderr, "No 'ds18cmd' setting in configuration file.\n");
		return(EXIT_FAILURE);
	} else {
		printf("ds18cmd: %s\n", ds18cmd);
	}


	// initialize message queue
	status = initMessageQueue(server, port, certificate);
	if (status != EXIT_SUCCESS) {
		printf("ERROR: Unable to connect to MQTT broker\n");
		return 1;
	}
	waitMessageQueueConnected();

	// check which device is to be used
	if (strcmp(argv[1], AMG8833_DEV_NAME) == 0) {
		status = amg8833_device(identifier, topic);
	}
	else if (strcmp(argv[1], ENV_COMBO_DEV_NAME) == 0) {
		status = envComboDevice(identifier, topic, time, ds18cmd);
	}
	else if (strcmp(argv[1], VL53L1X_DEV_NAME) == 0) {
		status = vl53l1x_device(identifier, topic);
	}
	else {
		printf("ERROR: Unknown device!\n");
		printUsage(argv[0]);
	}

	destroyMessageQueue();
	return 0;
}