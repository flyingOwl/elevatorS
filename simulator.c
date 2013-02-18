/*
	Has an infinite loop to create and run passengers using the elvator
*/

#include "elevator.h"
#include "passenger.h"
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

int running = 1;
const int microToMilli = 1000;

int mainLoop(int passengerThreshold, int maxLevels, int maxWaiters, int maxPassengers, int elevatorSteps,int sleepMS){

	int newPassenger = 0;
	srand(time(NULL));
	initElevator(maxLevels, maxPassengers, elevatorSteps);
	initPassengers(maxWaiters, maxPassengers);

	while(running){
		simulateStep();
		newPassenger++;
		if(newPassenger >= passengerThreshold){

			int or = rand() % maxLevels, de;
			do {
				de = rand() % maxLevels;
			} while (de == or);
			createNew(or, de);
			newPassenger = 0;
		}
		usleep(sleepMS * microToMilli);
	}
	return running; //should be "0"
}

int stopSimulation(){
	running = 0;
	return 0;
}
