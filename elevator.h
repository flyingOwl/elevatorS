#define ELEVATOR_SPEED 1
#define DIR_UP 1
#define DIR_NONE 0
#define DIR_DOWN -1
#define MAX_PASSENGERS 12
#define MAX_CONTROLLER_LIST 1000

int initElevator(int totalLs, int maxPs, int steps);
int simulateStep();
int callElevator(int fromLevel);
int pressLevelButton(int toLevel);
int addPassengers(int nPassengers);
int removePassengers(int nPassengers);
int getFreeSpace();
int getPassengers();
double getCurrentLevel();
int getMovement();

//internal functions:
int reachLevel(int atLevel);
int openDoors(int atLevel);
int calcMovement();
int getTotalLevels();
int getCurrentLevelRound();
int getPriorityAtLevel(int level);
