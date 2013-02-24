#include "elevator.h"
#include <stdlib.h>
#include "passenger.h"

double currentLevel; //double, -> 1/2 level
int destinationLevel;
int currentMovement;

int * calledLevels;
int * pressedLevels;
int * priorityLevels;
int totalLevels;

int maxPassengers;
int currentPassengers;
double moveSpeed;

int initElevator(int totalLs, int maxPs, int steps){
	calledLevels   = malloc(totalLs * sizeof(int));
	pressedLevels  = malloc(totalLs * sizeof(int));
	priorityLevels = malloc(totalLs * sizeof(int));
	if(!calledLevels || !pressedLevels){
		return 1;
	}
	int i = 0;
	for(; i < totalLs; i++){
		calledLevels[i]   = 0;
		pressedLevels[i]  = 0;
		priorityLevels[i] = 0;
	}
	totalLevels   = totalLs;
	maxPassengers = maxPs;
	currentPassengers = 0;
	currentMovement   = 0;
	moveSpeed = 1 / (double) steps;
	return 0;
}

int getDestinationCount(){
	//returns the number of levels pressed on the "pressedLevels"-board
	//inside the elevator
	// if = 0 -> elevator "empty"
	int i = totalLevels, temp = 0;
	while(i--){
		if(pressedLevels[i]){
			temp++;
		}
	}
	return temp;
}

int simulateStep(){
//	currentMovement = calcMovement();
	currentLevel += (currentMovement * moveSpeed);
	int cTemp = (int) currentLevel;
	if(cTemp == currentLevel){
		//reached a level...
		if(!reachLevel(cTemp)){
			openDoors(cTemp);
		}
	}
	return 0;
}

int priorityPlus(){
	int i = 0;
	for(; i < totalLevels; i++){
		if(calledLevels[i]){
			priorityLevels[i]++;
		}
	}
	return 0;
}

int callElevator(int fromLevel){
	//add level to List of waiting-queue
	calledLevels[fromLevel] = 1;
	if(!currentMovement){
		currentMovement = calcMovement();
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
	for(; current >= 0 && current < totalLevels; current += direction){
		if(calledLevels[current] || pressedLevels[current]){
			return 1;
		}
	}
	return 0;
}

int absDiff(int num1, int num2){
	return (num1 > num2) ? (num1 - num2) : (num2 - num1);
}

int calcMovement(){
	//calculate level to move to

	//first dummy implementation:
	int nextLevel;
	priorityPlus();
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
			//get closest level:
			int temp = totalLevels, i = 0;
			for(; i < totalLevels; i++){
				if((pressedLevels[i] || calledLevels[i]) && (absDiff(currentLevel,i) < temp)){
					temp = i;
				}
			}
			return (temp > currentLevel) ? DIR_UP : DIR_DOWN;			
		}
	}
	return 0;
}

int reachLevel(int atLevel){
	//return: stop or not to stop
	if(atLevel < 0 || atLevel >= totalLevels){
		return 1;
	}
	if(pressedLevels[atLevel]){
		return 0;
	}
	if(calledLevels[atLevel]){
		//stop here, or does another level in this direction waits longer?
		int i = atLevel + currentMovement, tMax = 0;
		for(; i < totalLevels && i >= 0 && currentMovement; i += currentMovement){
			if(priorityLevels[i] > tMax){
				tMax = priorityLevels[i];
			}
		}
		if(tMax > priorityLevels[atLevel]){
			return 1;
		} else {
			return 0;
		}
	} else {
		//at this level, nobody gives a shit
		return 1;
	}
}

int openDoors(int atLevel){
	//reset all calls at/to this level
	if(atLevel < 0 || atLevel >= totalLevels){
		return 1;
	}
	pressedLevels[atLevel]  = 0;
	calledLevels[atLevel]   = 0;
	priorityLevels[atLevel] = 0;
	//call passenger control -> add/remove them
	if(!atLevel || atLevel == (totalLevels - 1) || !(getDestinationCount())){
		currentMovement = 0;
	}
	passengerLevel(atLevel,currentMovement);
	if(!currentPassengers){
		currentMovement = 0;
	}
	currentMovement = calcMovement();
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

int getTotalLevels(){
	return totalLevels;
}

int getCurrentLevelRound(){
	return (int) (currentLevel + 0.5);
}

double getCurrentLevel(){
	return currentLevel;
}

int getMovement(){
	return currentMovement;
}
