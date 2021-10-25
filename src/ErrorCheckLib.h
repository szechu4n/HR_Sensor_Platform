typedef uint16_t GlobalErrorType;
GlobalErrorType globalErrorCode = 0;

void ErrorSet(GlobalErrorType&, ErrorCode); // sets errors as they occur 
ErrorCode ErrorGet(GlobalErrorType*); // processes errors
void ErrorPrint(GlobalErrorType*); // prints errors 
void ErrorClear(GlobalErrorType*); // clears errors