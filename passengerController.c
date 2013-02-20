#include "passenger.h"
#include "elevator.h"
#include <stdlib.h>

struct passengerInside **  inSider;
int mInside;
struct passengerWaiting ** waiters;
int mWaiting;

int initPassengers(int maxWaiters, int maxPassengers){
	inSider = malloc(maxPassengers * sizeof(struct passengerInside *));
	waiters = malloc(maxWaiters * sizeof(struct passengerWaiting *));
	mInside = maxPassengers;
	mWaiting = maxWaiters;
	if(!inSider || !waiters){
		return 1;
	}
	int i = 0;
	for(; i < maxWaiters; i++){
		waiters[i] = 0;
	}
	for(i = 0; i < maxPassengers; i++){
		inSider[i] = 0;
	}
	return 0;
}

int createNew(int fromFloor, int toFloor){
	//find free "slot"
	if(fromFloor == toFloor){
		return 1;
	}
	int i = 0;
	while(waiters[i]){
		i++;
	}
	waiters[i] = malloc(sizeof(struct passengerWaiting));
	if(waiters[i]){
		waiters[i]->origin = fromFloor;
		waiters[i]->destination = toFloor;
		callElevator(fromFloor);
		return 0;
	}
	return 1;
}

int passengerLevel(int level, int direction){
	//called when elevator opens
	//take out and put in new passengers

	//Take out all passengers Inside with destination = currentLevel
	int i = 0, count = 0;
	for(; i < mInside; i++){
		if(inSider[i] && inSider[i]->destination == level){
			count++;
			free(inSider[i]);
			inSider[i] = 0;
		}
	}
	//remove number from elevator:
	removePassengers(count);

	//Add new passengers:
	int cNew = getFreeSpace(); //...in elevator
	count = 0;
	for(i = 0; i < mWaiting && count < cNew; i++){
		if(waiters[i] && waiters[i]->origin == level){
			//found passenger -> he's standing at opened elevator:
			//get in if elevator is standing there; moving in the right direction; is empty
			if((!direction || ((waiters[i]->destination - level) * direction) > 0 ) || !getPassengers()){
				//yes -> right direction:
				count++;
				movePassengerInside(waiters[i]);
				free(waiters[i]);
				waiters[i] = 0;

			} else {
				//no -> wrong direction:
				callElevator(level);
			}

		}
	}
	//add to elevator:
	if(addPassengers(count)){
		//elevator is FULL ... wait what??
	}
	return 0;
}

int movePassengerInside(struct passengerWaiting * myPassenger){
	int i = 0;
	while(inSider[i]){
		i++;
	}
	inSider[i] = malloc(sizeof(struct passengerInside));
	if(inSider[i]){
		inSider[i]->destination = myPassenger->destination;
		pressLevelButton(myPassenger->destination);
		return 0;
	}
	return 1;
}

int getWaitersAtLevel(int level){
	int temp = 0, i = 0;
	for(; i < mWaiting; i++){
		if(waiters[i] && waiters[i]->origin == level){
			temp++;
		}
	}
	return temp;
}
