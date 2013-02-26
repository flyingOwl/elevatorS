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
			//TODO: Create headerfile with help texts

			//Analyse and do something with this line...
			if(!strcmp(line,"help")){
				outputLine("Commands: 'new' 'quit'   Call 'help <command>' for more information");
				continue;
			}
			if(!strncmp(line,"help",4)){
				//something was called with help
				line += 5;
				if(!strcmp(line,"new")){
			outputLine("new A B (C): Creates 1 or C (if given) new passenger(s) from level A to B. Example: \"new 0 2\" \"new 2 4 5\"");
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
				int a, b, c;
				switch(sscanf(line,"%d %d %d",&a,&b,&c)){
					case 2: {
						createNew(a,b);
						break;
					}
					case 3: {
						while(c--){
							createNew(a,b);
						}
						break;
					}
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

