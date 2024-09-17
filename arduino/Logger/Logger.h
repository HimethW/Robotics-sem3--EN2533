/*
  Logger.h
  EN2533 - Robot Design and Competition
  Himeth Walgampaya, 17 September, 2024
*/

#ifndef Logger_h
#define Logger_h

#include "Arduino.h"

class Logger {
  public:
    // Method to log a message with file, line, function, and time
    static void log(const char* message, const char* file, int line, const char* function);
  
  private:
    // Method to print the current time (in milliseconds since start)
    static void printTime();
};

// Macro to simplify logging by automatically passing file, line, and function info
#define LOG_MESSAGE(msg) Logger::log(msg, __FILE__, __LINE__, __FUNCTION__)

#endif
