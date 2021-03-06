/*
	Has an infinite loop to create and run passengers using the elvator
*/

#include "elevator.h"
#include "passenger.h"
#include <unistd.h>
#include <stdlib.h>

int running = 1, autoNew = 1;
const int microToMilli = 1000;

int mainLoop(int passengerThreshold, int maxLevels, int maxWaiters, int maxPassengers, int elevatorSteps,int sleepMS, int doorTime){

	int newPassenger = 1;
	initElevator(maxLevels, maxPassengers, elevatorSteps, doorTime);
	initPassengers(maxWaiters, maxPassengers, maxLevels);

	while(running){
		simulateStep();
		newPassenger += (autoNew) ? 1 : 0;
		if(newPassenger > passengerThreshold){
			createRandom(1);
			newPassenger = 1;
		}
		usleep(sleepMS * microToMilli);
	}
	return running; //should be "0"
}

int stopSimulation(){
	running = 0;
	return 0;
}

int toggleAutonew(int setOn){
	autoNew = setOn;
	return 0;
}
