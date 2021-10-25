#include "main.h"

void ErrorSet(GlobalErrorType& globalErrorCode, ErrorCode error){
    globalErrorCode = 1 << error;
}