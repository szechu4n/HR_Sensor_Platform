#include "main.h"

uint8_t isRunInProgress;
IntervalTimer processTimer;

void CmdInvoker(uint8_t msg[]){
/*
    CmdInvoker is a wrapper function that calls commands from
    the command table in "SystemCommands.h". It uses function
    pointers to call the specific commands required.
*/
    int i = 0;
    while(commands[i].header != 0)
    {
        if(commands[i].header == msg[0]){ // reminder to do a check of the integrity of the data
            commands[i].execute(msg);
            break;
        }
        i++;
    }
}

void CmdVersion(uint8_t msg[]){
/*
    Simple test to ensure software is correct version
*/
    if(isRunInProgress)
    {
        return;
    }
    uint8_t new_msg[5];
    new_msg[0] = 0x61;
    new_msg[1] = uint8_t(VER_MAJOR);
    new_msg[2] = uint8_t(VER_MINOR);
    new_msg[3] = uint8_t(VER_DEBUG);
    new_msg[4] = CRCFast(new_msg, 4);
    Serial.write(new_msg,4);
}

void CmdSDTest(uint8_t msg[]){
/*
    Simple test to ensure system works
*/
}

void CmdBlinkLED(uint8_t msg[]){
    if(isRunInProgress)
    {
        return;
    }
    digitalWrite(ledSerialPin_D7, LOW);
    delay(250);
    digitalWrite(ledSerialPin_D7, HIGH);
}

void CmdDataCollect(uint8_t msg[]){
/*
    System collects large amount of data and stores in SD card.
    Once SD Card is full (definition of full being 30 minutes of data),
    transmits data over UART for testing. Keeps data in case filtering is desired.
*/
    // enable RunInProgress flag
    // start RTC with message data
    // track start time, later compare start time with current time to see if 30 minutes is up
    // set flag if filtering is enabled in this message
/*
    System takes large amount of data in SD card and filters with built in filters.
    Once filtering is done, transmits data over UART for testing. Removes data when done
*/
    // setup timer and adc interrupt connection
    // while run in progress
    // set to sleep
}

void CmdDataFilter(uint8_t msg[]){ // may be unnecessary

}

void CmdRun(uint8_t msg[]){
    isRunInProgress = 1;
    uint32_t rtcData = 0;
    uint8_t *rtcData_ptr = (uint8_t*)&rtcData;
    for(int i = 4; i > 0; i++){
        rtcData_ptr[i-1] = msg[5-i];
    }
    time_t t = (time_t)rtcData;
    Teensy3Clock.set(t);
    processTimer.begin(ProcessSystem,5000);
}

void CmdRunStop(uint8_t msg[]){
    isRunInProgress = 0;
    processTimer.end();
    // disable RunInProgress flag
    // turn off timer and adc interrupt
}

void CmdSync(uint8_t msg[]){ // probably unnecessary
    uint32_t rtcData = 0;
    uint8_t *rtcData_ptr = (uint8_t*)&rtcData;
    for(int i = 4; i > 0; i++){
        rtcData_ptr[i-1] = msg[5-i];
    }
    time_t t = (time_t)rtcData;
    time_t t_ = Teensy3Clock.get();
    if(t_ != t)
        Teensy3Clock.set(t);
}