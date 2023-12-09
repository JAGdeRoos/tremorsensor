/******************************************************************************
 * Project    : Tremor Sensor Inator
 * Author     : Remko Welling
 *
 * Description: Header file for timer implementation from IoT class
******************************************************************************/

#ifndef __PE1MEW_TIMER_H_
#define __PE1MEW_TIMER_H_

#include <stdint.h>
#include <Arduino.h>  /* for millis() */

class PE1MEW_Timer
{
  
  public:
  protected:
  private:

    
    unsigned long _msEndTime;                         /* end time at which timer shall expire */
    bool _active;                                     /* State of the timer, true = running, false = expired. */
    static const uint32_t msTimeMax = 4294967295;     /* Maximum value of a 32 bit unsigned integer 2^32 − 1 */
  
  public:
    /*! @brief the standard constructor */
    PE1MEW_Timer      ();
    /*! @brief the destructor */
    ~PE1MEW_Timer     ();

    /*! @brief Set time in milliseconds in the future from now that the time shall expire
     * @param ms deadline
     * @return true if timer set successfully, false if timer is running and not set. */
    bool setExpiry    (uint32_t msTime);

    /*! @brief Test if the timer has expired
     * @return true if timer has expired, false if not */
    bool getExpired   ();

  protected:
  private:
}; /* PE1MEW_Timer */

#endif /*__PE1MEW_TIMER_H_ */
