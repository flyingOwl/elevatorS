#define LEVEL_SECTION 3
#define WAITING_SECTION 14
#define PRIORITY_SECTION 17
#define ELEVATOR_SECTION 21
#define MOVE_SECTION 25

#define LINE_IN 8
#define LINE_OUT 7

int interfaceLoop(int loopWait, int maxLevels);
int cleanInputLine();
int outputLine(const char * buffer);
int stopInterface();

