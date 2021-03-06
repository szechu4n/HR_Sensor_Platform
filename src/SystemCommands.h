#ifndef SYSTEMCOMMANDS_H
#define SYSTEMCOMMANDS_H

typedef void(*functionPointerType)(uint8_t[]);
struct commandStruct{
    char const *name;
    uint8_t const header;
    functionPointerType execute;
    char const *help;
}; // Abstract Implementation of the Command Object as a C Struct


void CmdInvoker(uint8_t[]);
void CmdVersion(uint8_t[]);
void CmdSDTest(uint8_t[]);
void CmdBlinkLED(uint8_t[]);
void CmdDataCollect(uint8_t[]);
void CmdDataFilter(uint8_t[]);
void CmdRun(uint8_t[]);
void CmdRunStop(uint8_t[]);
void CmdSync(uint8_t[]);

//
const struct commandStruct commands[] = {
    {"ver", 0x61, &CmdVersion,
     "Display Firmware Version"},
/*    {"sdTest", 2, &CmdSDTest,
     "Runs SD Card test, returns number of errors on completion."},
    {"blinkLed", 3, &CmdBlinkLED,
     "Blinks LED at desired frequency."},
*/    {"dataCollect", 0x31, &CmdDataCollect,
     "Collects unfiltered data and transmits data over serial."},
    /*{"filterTest", 5, &CmdDataFilter,
     "Collects and filters data, and transmits results over serial."},*/
    {"sync", 0x65, &CmdSync,
     "Sets system to deepest sleep available."},
    {"run", 0x71, &CmdRun,
     "Runs the basic functionality. Exits Debug Idle Mode."}, // need to evaluate in terms of header and such
    {"runstop", 0x81, &CmdRunStop,
     "Stops system running. Returns to Idle Mode."},
    {"",0,0,""} // End of Table indicator
}; // Concrete Implementations of several Command Objects

#endif
