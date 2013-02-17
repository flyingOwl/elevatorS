#include "elevator.h"
#include "queue.h"

int etagenGesamt;
int currentEtage;

int opMode;

struct queueEntry {
	int waitLevel;
	int direction;
	struct queueEntry * nextEntry;
};

struct queueEntry * queueHead;


int addToQueue(int currentLevel, int direction){
	if(queueHead){
		struct queueEntry * tempEntry = queueHead;
		while(tempEntry->nextEntry){
			tempEntry = tempEntry->nextEntry;
		}
		tempEntry->nextEntry = malloc(sizeof(struct queueEntry));
		tempEntry = tempEntry->nextEntry;
		if(tempEntry){
			tempEntry->nextEntry = 0;
			tempEntry->waitLevel = currentLevel;
			tempEntry->direction = direction;
		} else {
			// got NULL-Pointer from malloc

			return 2;
		}
	} else {
		queueHead = malloc(sizeof(struct queueEntry));
		queueHead->nextEntry = 0;
		queueHead->waitLevel = currentLevel;
		queueHead->direction = direction;
	}
	return 0;
}

int getNextStop(int direction){
	switch(opMode){
		case MODE_DUMB: {
			if(queueHead){
				return queueHead->waitLevel;
			} else {
				return -1;
			}
			break;
		}
		case MODE_KI1: {

			break;
		}
	}
	return -1;
}

int removeWaitersFromQueue(int level, 
