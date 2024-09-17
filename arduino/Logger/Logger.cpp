/*
  Logger.cpp
  EN2533 - Robot Design and Competition
  Himeth Walgampaya, 17 September, 2024
*/

#include "Logger.h"

void Logger::log(const char* message, const char* file, int line, const char* function) {
  // Print the time
  printTime();

  // Print the file name, line number, and function name
  Serial.print("[FILE: ");
  Serial.print(file);
  Serial.print("] [LINE: ");
  Serial.print(line);
  Serial.print("] [FUNCTION: ");
  Serial.print(function);
  Serial.print("] ");

  // Print the user-provided message
  Serial.println(message);
}

void Logger::printTime() {
  unsigned long currentTime = millis();
  Serial.print("[TIME: ");
  Serial.print(currentTime);
  Serial.print(" ms] ");
}
