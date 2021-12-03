#ifndef ERRORCHECK_H
#define ERRORCHECK_H

#include "main.h"

typedef uint16_t GlobalErrorType;

void ErrorSet(ErrorCode); // sets errors as they occur 
ErrorCode ErrorGet(); // processes errors
void ErrorPrint(); // prints errors 
void ErrorClear(); // clears errors

#endif