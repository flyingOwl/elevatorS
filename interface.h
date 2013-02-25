#define LEVEL_SECTION 3
#define WAITING_SECTION 14
#define ELEVATOR_SECTION 19
#define MOVE_SECTION 25

#define LINE_IN 2
#define LINE_OUT 3

int interfaceLoop(int loopWait, int maxLevels);
int cleanInputLine();
int outputLine(char * buffer);
int stopInterface();

