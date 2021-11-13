#include "main.h"

uint8_t isRunInProgress;

void CmdInvoker(uint8_t msg[], uint8_t idx){
    int i = 0;
    while(commands[i].name != "")
    {
        if(commands[i].name == msg[0]){ // reminder to do a check of the integrity of the data
            *commands[i].execute();
            return;
        }
        i++;
    }
}

void CmdVersion(){

}

void CmdSDTest(){

}

void CmdBlinkLED(){

}

void CmdDataCollect(){

}

void CmdDataFilter(){

}

void CmdRun(){

}

void CmdRunStop(){
    
}

void CmdDeepSleep(){

}