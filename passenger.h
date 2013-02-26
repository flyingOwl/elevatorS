struct passengerWaiting {
	int origin;
	int destination;
};

struct passengerInside {
	int destination;
};

int initPassengers(int maxWaiters, int maxPassengers, int maxLevels);
int createRandom(int nTimes);
int createNew(int fromFloor, int toFloor);
int passengerLevel(int level, int direction);

int getWaitersAtLevel(int level);

int movePassengerInside(struct passengerWaiting * myPassenger);
