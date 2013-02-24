all: elevatorControl.o passengerController.o simulator.o main.o interface.o
	gcc -g -o elevatorSimulator elevatorControl.o passengerController.o simulator.o main.o interface.o -lpthread

elevatorControl.o: elevatorControl.c
	gcc -Wall -g -c -o elevatorControl.o elevatorControl.c

passengerController.o: passengerController.c
	gcc -Wall -g -c -o passengerController.o passengerController.c

simulator.o: simulator.c
	gcc -Wall -g -c -o simulator.o simulator.c

interface.o: interface.c
	gcc -Wall -g -c -o interface.o interface.c

main.o: main.c
	gcc -Wall -g -c -o main.o main.c -lpthread

clean:
	rm -f *.o
	rm -f elevatorSimulator
