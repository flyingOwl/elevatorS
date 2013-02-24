#include "simulator.h"
#include "interface.h"
#include <pthread.h>
#include <stdlib.h>

void *startInterfaceLoop(void *ptr){
	int * arg = (int *) ptr;
	interfaceLoop(arg[0],arg[1]);
	return NULL;
}

void * startCoreLoop(void *ptr){
	int * arg = (int *) ptr;
	mainLoop(arg[0],arg[1],arg[2],arg[3],arg[4],arg[5]);
	return NULL;
}

int main(int argc, char ** argv){	
	int * pArgs = malloc(sizeof(int) * 6);
	pArgs[0] = 15;    //every x steps -> new passenger
	pArgs[1] = 8;     //house size (levels)
	pArgs[2] = 1000;  //malloc for waiters
	pArgs[3] = 12;    //elevator size (maxPassengers)
	pArgs[4] = 4;     //speed steps (1/x per second)
	pArgs[5] = 25;   //Core loop pause time in ms
	pthread_t pCore;
	pthread_create(&pCore, NULL, startCoreLoop, pArgs);

	int * pFac = malloc(sizeof(int) * 2);
	pFac[0] = 25;    //loop pause time in ms
	pFac[1] = 8;      //house size (levels)
	pthread_t pInterface;
	pthread_create(&pInterface, NULL, startInterfaceLoop, pFac);
	
	pthread_join(pInterface,NULL);
	//pthread_join(pCore,NULL);
	stopSimulation();
	return 0;
}

