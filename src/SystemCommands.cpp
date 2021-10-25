#include "main.h"

void CmdInvoker(uint8_t msg[], uint8_t idx){
    int i = 0;
    while(commands[i].name != "")
    {
        if(commands[i].name == msg[0]){
            *commands[i].execute();
            return;
        }
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

void CmdDeepSleep(){

}