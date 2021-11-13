
typedef void(*functionPointerType)(void);
struct commandStruct{
    char const *name;
    functionPointerType execute;
    char const *help;
}; // Abstract Implementation of the Command Object as a C Struct


void CmdInvoker(uint8_t[], uint8_t);
void CmdVersion();
void CmdSDTest();
void CmdBlinkLED();
void CmdDataCollect();
void CmdDataFilter();
void CmdRun();
void CmdRunStop();
void CmdDeepSleep();

//
const struct commandStruct commands[] = {
    {"ver", &CmdVersion,
     "Display Firmware Version"},
    {"sdTest", &CmdSDTest,
     "Runs SD Card test, returns number of errors on completion."},
    {"blinkLed", &CmdBlinkLED,
     "Blinks LED at desired frequency."},
    {"dataCollect", &CmdDataCollect,
     "Collects unfiltered data and transmits data over serial."},
    {"filterTest", &CmdDataFilter,
     "Collects and filters data, and transmits results over serial."},
    {"deepSleep", &CmdDeepSleep,
     "Sets system to deepest sleep available."},
    {"run", &CmdRun,
     "Runs the basic functionality. Exits Debug Idle Mode."}, // need to evaluate in terms of header and such
    {"runstop", &CmdRunStop,
     "Stops system running. Returns to Idle Mode."},
    {"",0,""} // End of Table indicator
}; // Concrete Implementations of several Command Objects


