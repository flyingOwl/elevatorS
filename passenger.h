struct passengerWaiting {
	int origin;
	int destination;
};

struct passengerInside {
	int destination;
};

int initPassengers(int maxWaiters, int maxPassengers);
int createNew(int fromFloor, int toFloor);
int passengerLevel(int level);

int movePassengerInside(struct passengerWaiting * myPassenger);