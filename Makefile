all: elevatorControl.o passengerController.o

elevatorControl.o: elevatorControl.c
	gcc -c -o elevatorControl.o elevatorControl.c

passengerController.o: passengerController.c
	gcc -c -o passengerController.o passengerController.c

clean:
	rm -f *.o
