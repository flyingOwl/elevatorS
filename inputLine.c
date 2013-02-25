#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inputLine.h"
#include "interface.h"
#include "passenger.h"

int rInput;

char * readLine(char * buffer, size_t length){
	char * p = fgets(buffer, length, stdin);
	if(p){
		size_t last = strlen(buffer) - 1;
		if(buffer[last] == '\n'){
			buffer[last] = '\0';
		} else {
			//buffer full, but no "new line" ->
			//discard all following input:
			fscanf(stdin,"%*[^\n]");
			(void) fgetc(stdin);
		}
	}
	return p;
}

int clearBuffer(char * buffer){
	int i = 0;
	for(; i < BUFFER_SIZE; i++){
		buffer[i] = '\0';
	}
	return 0;
}

int stopInputLoop(){
	rInput = 0;
	return 0;
}

int inputLoop(){
	char * buffer = malloc(sizeof(char) * BUFFER_SIZE);
	rInput = 1;
	while(rInput){
		clearBuffer(buffer);
		cleanInputLine();
		char * line = readLine(buffer,BUFFER_SIZE);
		if(line){
			//Analyse and do something with this line...
			if(!strcmp(line,"help")){
				outputLine("Commands: 'new' 'quit'   Call 'help <command>' for more information");
				continue;
			}
			if(!strncmp(line,"help",4)){
				//something was called with help
				line += 5;
				if(!strcmp(line,"new")){
					outputLine("new A B: Creates a new passenger from level A to B. Example: new 0 2");
					continue;
				}
				if(!strcmp(line,"quit")){
					outputLine("quit: Stops simulation and quits.");
					continue;
				}
				// ... more help
				continue;
			}
			// NEW PASSENGER
			if(!strncmp(line,"new",3)){
				line += 4;
				int a, b;
				if(sscanf(line,"%d %d",&a,&b) == 2){
					createNew(a,b);
				}
				continue;
			}
			// QUIT
			if(!strcmp(line,"quit")){
				stopInputLoop();
				continue;
			}
		}
	}
	free(buffer);
	return rInput;
}

