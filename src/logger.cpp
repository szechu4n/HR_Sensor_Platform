#include "logger.h"

Logger::Logger()
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

Logger::Logger(char * fileName)
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
    m_file.println("---------- Start of Log ----------");
    m_file.close();
}

void Logger::setFileName(char * fileName)
{
    m_filename = fileName;
    m_lineNum = 0;
}

void Logger::logEvent(const char * str)
{
    m_file = sd.open(m_filename, FILE_WRITE);
    m_file.println("|" + String(m_lineNum++) + "| " + str);
    m_file.close();
}

void Logger::logEvent(uint8_t msg[], int len)
{
    m_file = sd.open(m_filename, FILE_WRITE);
    m_file.print("|" + String(m_lineNum++) + "| ");
    for(int i = 0; i < len-1; i++)
    {
        m_file.print(msg[i], DEC);
    }
    m_file.println(msg[len-1], DEC);
    m_file.close();
}