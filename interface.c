#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"
#include "elevator.h"
#include "passenger.h"
#include "inputLine.h"

int totalLevels, running;
int cleanInput = 0;
char * outBuffer = 0;

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

int printOutputLine(){
	putCursor(LINE_OUT + totalLevels,0);
	clearLine();
	if(outBuffer){
		printf("%s", outBuffer);
	}
	return 0;
}

int initInput(){
	putCursor(LINE_IN + totalLevels,0);
	clearLine();
	putchar('>');
	putCursor(LINE_IN + totalLevels,3);
	return 0;
}

int initInterface(){
	const int yAxis = 5;
	putCursor(yAxis,LEVEL_SECTION);
	outBuffer = malloc(sizeof(char) * BUFFER_SIZE);
	outputLine("Call 'help' for available commands");
	int i = 1;
	for(; i <= totalLevels; i++){
		printf("Level %d:",(totalLevels - i));
		putCursor(i+yAxis,LEVEL_SECTION);
	}
	return 0;
}

int printWaiters(){
	const int yAxis = 5;
	int i = 1;
	for(; i <= totalLevels; i++){
		putCursor(i+yAxis-1,LEVEL_SECTION);
		clearLine();
		printf("Level %d:",(totalLevels - i));
		putCursor(i+yAxis-1,WAITING_SECTION);
		printf("%d",getWaitersAtLevel(totalLevels -i));
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

int printMovement(){
	const int yAxis = 5 + totalLevels;
	double elevPos = getCurrentLevel();
	int move = getMovement();
	putCursor(yAxis, LEVEL_SECTION);
	clearLine();
	printf("Elevator moving (%d): %f",move,elevPos);
	return 0;
}

int stopInterface(){
	running = 0;
	return 0;
}
		
int interfaceLoop(int loopWait, int maxLevels){
	running = 1;
	int updated = 0;
	clearScreen();
	totalLevels = maxLevels;
	initInterface();
	fflush(stdout);
	while(running){
		//save cursor:
		if(cleanInput){
			initInput();
			cleanInput = 0;
		}
		printf("\033[s");

		putCursor(0,0);
		clearLine();
		printf("#%d\n",updated++);
		
		printOutputLine();
		
		printWaiters();

		printElevator();

		printMovement();

		//restore cursor:
		printf("\033[u");
		fflush(stdout);

		usleep(loopWait * 1000);
	}
	clearScreen();
	putCursor(0,0);
	fflush(stdout);
	return 0;
}

int cleanInputLine(){
	cleanInput = 1;
	return 0;
}

int outputLine(const char * buffer){
	if(outBuffer){
		outBuffer = strncpy(outBuffer,buffer,BUFFER_SIZE);
	}
	return 0;
}
