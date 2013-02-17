#include "passenger.h"
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
	waiters[i]->origin = fromFloor;
	waiters[i]->destination = toFloor;
	return 0;
}

int passengerLevel(int level){
	//called when elevator opens
	//take out and put in new passengers

	//Take out all passengers Inside with destination = currentLevel
	int i = 0, count = 0;
	for(; i < mInside; i++){
		if(inSider[i]->destination == level){
			count++;
			inSider[i] = 0;
		}
	}
	//remove number from elevator:
	removePassengers(count);

	//Add new passengers:
	int cNew = getFreeSpace(); //...in elevator
	count = 0;
	for(i = 0; i < mWaiting && count < cNew; i++){
		if(waiters[i]->origin == level){
			count++;
			movePassengerInside(waiters[i]);
			waiters[i] = 0;
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
	
	inSider[i]->destination = myPassenger->destination;
	return 0;
}
