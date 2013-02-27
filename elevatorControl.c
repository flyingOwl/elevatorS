#include "elevator.h"
#include <stdlib.h>
#include "passenger.h"

double currentLevel; //double, -> 1/2 level
int currentMovement;
int keepDirection;
int fullStopLevel;

int * calledLevels;
int * pressedLevels;
int totalLevels;

int maxPassengers;
int currentPassengers;
double moveSpeed;

int initElevator(int totalLs, int maxPs, int steps){
	calledLevels   = malloc(totalLs * sizeof(int));
	pressedLevels  = malloc(totalLs * sizeof(int));
	if(!calledLevels || !pressedLevels){
		return 1;
	}
	int i = 0;
	for(; i < totalLs; i++){
		calledLevels[i]   = 0;
		pressedLevels[i]  = 0;
	}
	totalLevels   = totalLs;
	maxPassengers = maxPs;
	currentPassengers = 0;
	currentMovement   = 0;
	keepDirection = 0;
	fullStopLevel = -1;
	moveSpeed = 1 / (double) steps;
	return 0;
}

int isInteger(double num){
	//return 1 -> is Integer or 0 -> not Integer
	return (num - ((int) num)  == 0);
}

int isIdle(int justEmpty){
	//returs if elevator is called to any level
	int i = -1;
	while(++i < totalLevels){
		if(pressedLevels[i] || (calledLevels[i] && !justEmpty)){
			return 0; //is not idle
		}
	}
	return 1; // is idle
}

int priorityPlus(){
	int i = 0;
	for(; i < totalLevels; i++){
		if(calledLevels[i]){
			calledLevels[i]++;
		}
		if(pressedLevels[i]){
			pressedLevels[i]++;
		}
	}
	return 0;
}

int simulateStep(){
	currentLevel += (currentMovement * moveSpeed);
	int cTemp = (int) currentLevel;
	if(isInteger(currentLevel)){
		//reached a level...
		if(!reachLevel(cTemp)){
			openDoors(cTemp);
		}
	}
	return 0;
}


int callElevator(int fromLevel){
	//add level to List of waiting-queue
	calledLevels[fromLevel] = 1;
	if(isIdle(1)){
		currentMovement = calcMovement();
	}
	return 0;
}

int pressLevelButton(int toLevel){
	//level-button inside elevator pressed, when passengers enter
	if(!pressedLevels[toLevel]){
		pressedLevels[toLevel] = 1;
	}
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
	if(!isInteger(currentLevel)){
		return currentMovement;
	}
	//else -> isInteger
	if(keepDirection){
		nextLevel = keepDirection;
		keepDirection = 0;
		return nextLevel;
	}
		
	int cLevel = (int) currentLevel;

	switch(currentMovement){
		case DIR_UP:  //fall through...
		case DIR_DOWN: {
			nextLevel = cLevel + currentMovement;
			if(calledToLevels(nextLevel,currentMovement)){
				return currentMovement;
			} else {
				if(calledToLevels(nextLevel, -1 * currentMovement)){
					return (-1 * currentMovement);
				} else {
					return DIR_NONE;
				}
			}
			break;
		}
		case 0: {
			//get closest level
			int empty = isIdle(1), tWhile = 0, diff = 1, temp;
			while(tWhile != 2){
				tWhile = 0;	
				do{
					temp = currentLevel + diff;
					if(temp < 0 || temp > (totalLevels - 1)){
						tWhile++;
					} else {
						if((calledLevels[temp] && empty) || (pressedLevels[temp] && !empty)){
							return (diff > 0) ? DIR_UP : DIR_DOWN;
						}
					}
					diff *= -1;
				} while (diff < 0);
				diff++;
			}
			return DIR_NONE;
		}
	}
	return 0;
}

int reachLevel(int atLevel){
	//return: stop or not to stop
	//somebody wants to leave, or elevator can't go on in current direction
	if(pressedLevels[atLevel] || !atLevel || atLevel == (totalLevels - 1)){
		return 0;
	}
	if(calledLevels[atLevel]){
		//stop here, or does another level in this direction waits longer?
		//Behavior depends on emptyness:
		int empty = isIdle(1);
		if(empty){
			//get Level with max priority in current direction:
			int i = atLevel, mPriority = 0, mLevel = atLevel;
			for(; i < totalLevels && i >= 0 && currentMovement; i += currentMovement){
				if(calledLevels[i] > mPriority){
					mPriority = calledLevels[i];
					mLevel    = i;
				}
			}
			//when atLevel is just one level from the max-priority-level -> then stop
			//(but keep direction)
			if(mLevel == atLevel){
				return 0;
			} else {
	//		if(absDiff(mLevel,atLevel) < 3 && currentMovement){
				keepDirection = currentMovement;
				fullStopLevel = mLevel;
				return 0;
			}
			return 1;
		} else {
			//stop, but keep direction:
			if(currentMovement && calledToLevels(atLevel + currentMovement,currentMovement)){
				keepDirection = currentMovement;
			}
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
	//call passenger control -> add/remove them
	if(!atLevel || atLevel == (totalLevels - 1) || isIdle(1)){
		currentMovement = 0;
	}
	if(fullStopLevel == atLevel){
		fullStopLevel = -1;
		currentMovement = 0;
	}
	passengerLevel(atLevel,currentMovement);
	if(isIdle(0)){
		currentMovement = 0;
	} else {
		currentMovement = calcMovement();
	}
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

int getFullStopLevel(){
	return fullStopLevel;
}

int getPriorityAtLevel(int level){
	if(level >= 0 && level < totalLevels){
		return calledLevels[level];
	} else {
		return -1;
	}
}
