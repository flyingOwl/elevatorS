#include <unistd.h>
#include <stdio.h>
#include "interface.h"
#include "elevator.h"
#include "passenger.h"

int totalLevels;

int clearScreen(){
	printf("\033[H\033[J");
	return 0;
}

int putCursor(int x, int y){
	printf("\033[%d;%df", x, y);
	return 0;
}

int clearLine(){
	printf("\033[2K");
	return 0;
}

int lineDown(){
	printf("\033[B");
	return 0;
}

int initInterface(){
	const int yAxis = 5;
	putCursor(yAxis,LEVEL_SECTION);
	int i = 1;
	for(; i <= totalLevels; i++){
		printf("Level %d:",(totalLevels - i));
		putCursor(i+yAxis,LEVEL_SECTION);
	}
	return 0;
}

int printWaiters(){
	const int yAxis = 5;
	putCursor(yAxis, WAITING_SECTION);
	int i = 1;
	for(; i <= totalLevels; i++){
		printf("%d",getWaitersAtLevel(totalLevels -i));
		putCursor(i+yAxis,WAITING_SECTION);
	}
	return 0;
}

int printElevator(){
	const int yAxis = 5;
	int elevPos = getCurrentLevelRound(), nPassen = getPassengers(), i = 1;
	putCursor(yAxis, ELEVATOR_SECTION);
	for(; i <= totalLevels; i++){
		if(elevPos == (totalLevels - i)){
			printf("[%2d]",nPassen);
		} else {
			printf("     ");
		}
		putCursor(yAxis+i, ELEVATOR_SECTION);
	}
	return 0;
}
		
int interfaceLoop(int loopWait, int maxLevels){
	int running = 1, updated = 0;
	clearScreen();
	totalLevels = maxLevels;
	initInterface();
	while(running){
		putCursor(0,0);
		clearLine();
		printf("#%d\n",updated++);
		
		printWaiters();

		printElevator();

		usleep(loopWait * 1000);
	}
	return 0;
}
