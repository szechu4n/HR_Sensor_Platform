#include "main.h"

uint8_t isRunInProgress;

void CmdInvoker(uint8_t msg[], uint8_t idx){
/*
    CmdInvoker is a wrapper function that calls commands from
    the command table in "SystemCommands.h". It uses function
    pointers to call the specific commands required.
*/
    int i = 0;
    while(commands[i].name != "")
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
}

void CmdSDTest(uint8_t msg[]){
/*
    Simple test to ensure system works
*/
}

void CmdBlinkLED(uint8_t msg[]){
/*
    Simple test to ensure system works
*/

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
    // enable RunInProgress flag
    // start RTC with message data
    // setup timer and adc interrupt connection
    // while run in progress
    // set to sleep
}

void CmdRunStop(uint8_t msg[]){
    // disable RunInProgress flag
    // turn off timer and adc interrupt
}

void CmdDeepSleep(uint8_t msg[]){ // probably unnecessary

}