#include <unistd.h>
#include <stdio.h>
#include "interface.h"

int clearScreen(){
	
	return 0;
}


int interfaceLoop(int loopWait){
	int running = 1;
	while(running){
		clearScreen();
		printf("Interface Thread is running...\n");
		usleep(loopWait * 1000);
	}
	return 0;
}
