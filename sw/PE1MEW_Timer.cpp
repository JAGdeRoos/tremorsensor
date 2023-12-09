/******************************************************************************
 * Project    : Tremor Sensor Inator
 * Author     : Remko Welling
 * 
 * Description: Implementation file for timer, slightly adjusted for overflows, from IoT class
******************************************************************************/

#include "PE1MEW_Timer.h"

// default constructor
PE1MEW_Timer::PE1MEW_Timer():
  _msEndTime(0),
  _active(false)
{
}

// default destructor
PE1MEW_Timer::~PE1MEW_Timer()
{
}

bool PE1MEW_Timer::setExpiry(uint32_t msTime)
{
  int temporary = millis() + msTime;
  if (temporary > msTimeMax)
  {
    return false;
  }
  else
  {
    _msEndTime = temporary;
    _active = true;
    return true;
  }
}


bool PE1MEW_Timer::getExpired()
{
  if (millis() < _msEndTime)
  {
    return false;
  }
  else
  {
    _active = false;
    return true;
  }
}
