#include "ErrorCheckLib.h"

static GlobalErrorType globalErrorCode;


void ErrorSet(ErrorCode error){
    globalErrorCode = globalErrorCode||(1 << error);
}

ErrorCode ErrorGet(){
    return NO_ERROR;
} // processes errors

void ErrorPrint(){

} // prints errors 

void ErrorClear(){

} // clears errors