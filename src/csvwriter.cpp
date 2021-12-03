#include "csvwriter.h"

CSVWriter::CSVWriter()
{
    #if DEBUG == 1
    Serial.println("Creating Log...");
    #endif
    if(!sd.begin())
    {
        digitalWrite(ledErrorPin_D8, HIGH);
        return;
    }
}

CSVWriter::CSVWriter(char * fileName)
{
    m_filename = fileName;
    #if DEBUG == 2
    Serial.println("Creating Log...");
    #endif
    if(!sd.begin(BUILTIN_SDCARD))
    {
        #if DEBUG == 2
        Serial.println("Initialization of SD Card Failed.");
        #endif
        digitalWrite(ledErrorPin_D8, HIGH);
        return;
    }
    if(sd.exists(m_filename))
    {
        #if DEBUG == 2
        Serial.println("Previous log exists. Overwriting...");
        #endif
        sd.remove(m_filename);
    }
    

    m_file = sd.open(m_filename, FILE_WRITE);
    m_file.close();
}

void CSVWriter::setFileName(char * fileName)
{
    m_filename = fileName;
}

void CSVWriter::writeLine(const char * str)
{
    m_file = sd.open(m_filename, FILE_WRITE);
    m_file.println(str);
    m_file.close();
}

void CSVWriter::logDataEvent(double msg[], int len)
{
    m_file = sd.open(m_filename, FILE_WRITE);
    for(int i = 0; i < len-1; i++)
    {
        m_file.print(msg[i], DEC);
        m_file.print(",");
    }
    m_file.println(msg[len-1], DEC);
    m_file.close();
}