static const char * OPTION_HELP =
"Usage: %s [--level n] [--capacity n] [--speed n]\n\
\t--level n\tChanges the house size to n levels\n\
\t--capacity n\tChanges the elevator capacity to n passengers\n\
\t--speed n\tChanges the speed of the elevator to 1 / n levels per simulation step; must be a power of two (eg. 2,4,8,...)\n";
static const char * OPTION_NEED_NUMBER = "The %s option needs a number\n";
static const char * OPTION_NEED_VALUE  = "The %s option must be followed be a value\n";
static const char * OPTION_UNKNOWN     = "Unknown option: %s\n";
static const char * OPTION_VALUE_OVER_MAX = "The %s value is greater than the maximal value of %d\n";
static const char * OPTION_SPEED_WRONG_VALUE = "The speed option need a value > 0 and 2^n = value (eg. 1,2,4,8...)\n";
