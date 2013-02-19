#include "elevator.h"
#include <stdlib.h>
#include "passenger.h"

#include <stdio.h>

double currentLevel; //double, -> 1/2 level
int destinationLevel;
int currentMovement;

int * calledLevels;
int * pressedLevels;
int totalLevels;

int maxPassengers;
int currentPassengers;
double moveSpeed;

int initElevator(int totalLs, int maxPs, int steps){
	calledLevels  = malloc(totalLs * sizeof(int));
	pressedLevels = malloc(totalLs * sizeof(int));
	if(!calledLevels || !pressedLevels){
		return 1;
	}
	int i = 0;
	for(; i < totalLs; i++){
		calledLevels[i]  = 0;
		pressedLevels[i] = 0;
	}
	totalLevels   = totalLs;
	maxPassengers = maxPs;
	currentPassengers = 0;
	currentMovement   = 0;
	moveSpeed = 1 / (double) steps;
	    printf("ELEVATOR: moveSpeed = %.8f\n",moveSpeed);
	return 0;
}

int simulateStep(){
	currentMovement = calcMovement();
	currentLevel += (currentMovement * moveSpeed);
	int cTemp = (int) currentLevel;
	if(cTemp == currentLevel){
		//reached a level...
		if(reachLevel(cTemp) == 1){
			openDoors(cTemp);
		}
	}
	  printf("ELEVATOR: moving %d -> position = %f (%d passengers)\n",currentMovement,currentLevel,currentPassengers);
	return 0;
}

int callElevator(int fromLevel){
	//add level to List of waiting-queue
	calledLevels[fromLevel] = 1;
	
	//when elevator not moving -> move directly to called level:
	if(currentMovement == DIR_NONE){
		currentMovement = (currentLevel < fromLevel) ? DIR_UP : DIR_DOWN;
	}
	return 0;
}

int pressLevelButton(int toLevel){
	//level-button inside elevator pressed, when passengers enter
	pressedLevels[toLevel]++;
	return 0;
}

int calledToLevels(int current, int direction){
	//checks if there are called levels in given direction
	int temp = 0;
	for(; current >= 0 && current < totalLevels; current += direction){
		if(calledLevels[current] || pressedLevels[current]){
			temp = 1;
		}
	}
	return temp;
}

int calcMovement(){
	//calculate level to move to

	//first dummy implementation:
	int nextLevel;
	switch(currentMovement){
		case DIR_UP: {
			nextLevel = (int) (currentLevel + 1);
			if(calledToLevels(nextLevel,DIR_UP)){ 
				return DIR_UP;
			} else {
				if(calledToLevels(nextLevel,DIR_DOWN)){ 
					return DIR_DOWN;
				} else {
					return DIR_NONE;
				}
			}
		}
		case DIR_DOWN: {
			nextLevel = (int) currentLevel;
			if(calledToLevels(nextLevel,DIR_DOWN)){ 
				return DIR_DOWN;
			} else {
				if(calledToLevels(nextLevel,DIR_UP)){ 
					return DIR_UP;
				} else {
					return DIR_NONE;
				}
			}
		}
		case 0: {
			if(calledToLevels((int) currentLevel, DIR_UP)){
				return DIR_UP;
			} else {
				if(calledToLevels((int) currentLevel, DIR_DOWN)){
					return DIR_DOWN;
				}
			}
			return 0;
		}
	}
	return 0;
}

int reachLevel(int atLevel){
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
	calledLevels[atLevel]  = 0;
	//call passenger control -> add/remove them
	passengerLevel(atLevel,currentMovement);
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

int getFreeSpace(){
	return maxPassengers - currentPassengers;
}

int getPassengers(){
	return currentPassengers;
}
