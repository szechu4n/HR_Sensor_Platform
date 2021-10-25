#include "main.h"

void CmdInvoker(char const* str){
    int i = 0;
    while(commands[i].name != "")
    {
        if(commands[i].name == str){
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