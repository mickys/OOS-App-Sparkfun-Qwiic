#include "amg8833.h"
#include <stdlib.h>

#define MQ_HOST 	"localhost"
#define MQ_PORT 	1883

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
	return status;
}

int main(int argc, char *argv[]) {
	int status = 0;
	
	printf("Program version: %s %s\n", __DATE__, __TIME__);
	
	// initialize message queue
	status = initMessageQueue(MQ_HOST, MQ_PORT);
	if (status != EXIT_SUCCESS) {
		printf("ERROR: Unable to connect to MQTT broker\n");
		return 1;
	}
	waitMessageQueueConnected();

	status = amg8833_device();


	return 0;
}
