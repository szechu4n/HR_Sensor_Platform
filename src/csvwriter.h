#ifndef CSVWRITER_H
#define CSVWRITER_H

#include "main.h"

#define SD_FAT_TYPE 2 // supports FAT16, 32, and 64.


class CSVWriter {
    
public:
    CSVWriter();
    CSVWriter(char *);
    void setFileName(char *);
    void writeLine(const char *);
    void logDataEvent(double msg[], int len);
private:
    #if SD_FAT_TYPE == 0
    SdFat sd;
    File m_file;
    #elif SD_FAT_TYPE == 1
    SdFat32 sd;
    File32 m_file;
    #elif SD_FAT_TYPE == 2
    SdExFat sd;
    ExFile m_file;
    #elif SD_FAT_TYPE == 3
    SdFs sd;
    FsFile m_file;
    #else  // SD_FAT_TYPE
    #error Invalid SD_FAT_TYPE
    #endif  // SD_FAT_TYPE

    char* m_filename;
};

#endif