#include "amg8833.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {
	int status = 0;

	status = amg8833_setup();

	if (status == EXIT_SUCCESS) {
		status = amg8833_readPixels();
	}


	return 0;
}
