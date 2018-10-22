#include "amg8833.h"
#include <stdlib.h>

int amg8833_device() {
	int status;
	float *pixelData = malloc(AMG8833_NUM_PIXELS * sizeof(float));
	
	status = amg8833_setup();

	if (status == EXIT_SUCCESS) {
		status = amg8833_readPixels(pixelData);
	}
	
	free(pixelData);
	return status;
}

int main(int argc, char *argv[]) {
	int status = 0;
	
	printf("Program version: %s %s\n", __DATE__, __TIME__);

	status = amg8833_device();


	return 0;
}
