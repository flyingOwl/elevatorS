#include "simulator.h"
#include "interface.h"
#include "inputLine.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "options.h"

#define MAX_LEVELS 35

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

void * startInputLoop(void * ptr){
	inputLoop();
	return NULL;
}

int main(int argc, char ** argv){
	int passengerThreshold = 15;
	int houseSize = 8;
	int maxWaiters = 1000;
	int elevatorCapacity = 12;
	int elevatorSteps = 4; // 1/n level per second
	int loopTime = 200; //ms

	//parsing commandline arguments:
	int i = 1, hasNext;
	for(; i < argc; i++){
		hasNext = (i + 1) < argc;

		if(!strcmp(argv[i],"--help")){
			printf(OPTION_HELP,argv[0]);
			return 0;
		}

		// --level
		if(!strcmp(argv[i],"--level")){
			if(hasNext){
				int nLevel;
				if(sscanf(argv[i+1],"%d",&nLevel)){
					if(nLevel > MAX_LEVELS){
						printf(OPTION_VALUE_OVER_MAX,"level",MAX_LEVELS);
						return 2;
					} else {
						houseSize = nLevel;
						i++;
					}
				} else {
					printf(OPTION_NEED_NUMBER,"level");
					return 2;
				}
			} else {
				printf(OPTION_NEED_VALUE,"level");
				return 2;
			}
			continue;
		}

		//--capacity
		if(!strcmp(argv[i],"--capacity")){
			if(hasNext){
				int nCapacity;
				if(sscanf(argv[i+1],"%d",&nCapacity)){
					elevatorCapacity = nCapacity;
					i++;
				} else {
					printf(OPTION_NEED_NUMBER,"capacity");
					return 2;
				}
			} else {
				printf(OPTION_NEED_VALUE,"capacity");
				return 2;
			}
			continue;
		}

		printf(OPTION_UNKNOWN,argv[i]);
		return 3;
		
	}

	int * pArgs = malloc(sizeof(int) * 6);
	pArgs[0] = passengerThreshold;	//every x steps -> new passenger
	pArgs[1] = houseSize;		//house size (levels)
	pArgs[2] = maxWaiters;		//malloc for waiters
	pArgs[3] = elevatorCapacity;	//elevator size (maxPassengers)
	pArgs[4] = elevatorSteps;	//speed steps (1/x per second)
	pArgs[5] = loopTime;		//Core loop pause time in ms
	pthread_t pCore;
	pthread_create(&pCore, NULL, startCoreLoop, pArgs);
	
	pthread_t pInput;
	pthread_create(&pInput, NULL, startInputLoop, NULL);

	int * pFac = malloc(sizeof(int) * 2);
	pFac[0] = loopTime;	//loop pause time in ms
	pFac[1] = houseSize;	//house size (levels)
	pthread_t pInterface;
	pthread_create(&pInterface, NULL, startInterfaceLoop, pFac);
	
	pthread_join(pInput,NULL);
	//pthread_join(pCore,NULL);
	
	stopInterface();
	pthread_join(pInterface,NULL);
	
	stopSimulation();
	pthread_join(pCore,NULL);
	free(pArgs);
	free(pFac);
	return 0;
}

