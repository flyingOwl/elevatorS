#include "elevator.h"

double currentLevel; //double, -> 1/2 level
int destinationLevel;
int currentMovement;

int * calledLevels;
int * pressedLevels;
int totalLevels;

int maxPassengers;
int currentPassengers;
double moveSpeed;

int init(int totalLs, int maxPs, int steps){
	calledLevels  = malloc(totalLs * sizeof(int));
	pressedLevels = malloc(totalLs * sizeof(int));
	if(!calledLevels || !pressedLevels){
		return 1;
	}
	int i = 0;
	for(; i < totalLs; i++){
		calledLevels[i] = -1;
	}
	totalLevels   = totalLs;
	maxPassengers = maxPs;
	currentPassengers = 0;
	moveSpeed = 1 / steps;
	return 0;
}

int simulateStep(){
	currentLevel += (currentMovement * moveSpeed);
	int cTemp = (int) currentLevel;
	if(cTemp == currentLevel){
		//reached a level...
		if(reachLevel(cTemp)){
			openDoors(cTemp);
		}
	}
	currentMovement = calcMovement();
}

int callElevator(int fromLevel){
	//add level to List of waiting-queue
	
	calledLevels[fromLevel] = 1;
	return 0;
}

int pressLevelButton(int toLevel){
	//level-button inside elevator pressed, when passengers enter
	pressedLevels[toLevel]++;
	return 0;
}

int calcMovement(){
	//called from main every xy seconds
	//calculate level to move to

	//1. dummy implementation:
	int nextLevel, i, temp = 0;
	switch(currentMovement){
		case DIR_UP: {
			nextLevel = (int) (currentLevel + 1);
			for(i = nextLevel; i < totalLevels; i++){
				if(calledLevels[i] || pressedLevels[i]){
					temp = 1;
				}
			}
			return (temp) ? DIR_UP : DIR_DOWN;
		}
		case DIR_DOWN: {
			nextLevel = (int) (currentLevel);
			for(i = nextLevel; i >= 0; i++){
				if(calledLevels[i] || pressedLevels[i]){
					temp = 1;
				}
			}
			return (temp) ? DIR_UP : DIR_DOWN;
		}
	}
	return 0;
}

int reachLevel(int atlevel){
	//return: stop or not to stop
	if(calledLevels[atLevel] || pressedLevels[atLevel]){
		return 1;
	} else {
		return 0;
	}
}

int openDoors(int atLevel){
	//reset all calls at/to this level
	pressedLevels[atLevel] = 0;
	int i = 0;
	for(; i < totalLevels; i++){
		if(calledLevels[i] == atLevel){
			calledLevels[i] = -1;
			return 0;
		}
	}

	//call passenger control -> add/remove them
	passengerLevel(atLevel);

	return 0;
}

int addPassengers(int nPassengers){
	if(currentPassengers + nPassengers > maxPassengers){
		currentPassengers = maxPassengers;
		return (currentPassengers + nPassengers) - maxPassengers;
	} else {
		currentPassengers += nPassengers;
	}
	return 0;
}

int removePassengers(int nPassengers){
	currentPassengers -= nPassengers;
	return (currentPassengers < 0);
}
