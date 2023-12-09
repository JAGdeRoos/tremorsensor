/******************************************************************************
 * Project    : Tremor Sensor Inator
 * Author     : Jochem de Roos
 *
 * Description: Header file for biquad filter. Using filter coefficients from 
 * butterworth filter class
******************************************************************************/

#ifndef __BIQUAD_H_
#define __BIQUAD_H_

#include <stdint.h>
#include <Arduino.h>  /* for millis() */
#include "ringbuffer.h"

class biquad
{
  public:
    /*! @brief The constructor for a biquad filter.
     * @param buffer_in : ring buffer for previous inputs
     * @param buffer_out : ring buffer for previous outputs */
    biquad() : buffer_in(3), buffer_out(3)
    {
      buffer_in.reset();
      buffer_out.reset();
    }

    /*! @brief Setter for the coefficients.
     * @param c[7] : coefficients to be set */
    void              set_coefficients      (float c[7]);

    /*! @brief Print function for the coefficients.
     * @param c[7] : coefficients to be set */
    void              print_coefficients    ();

    /*! @brief filters the input signal using the coefficients
     * @param IMU axis input to be filtered 
     * @return filtered input */
    float             filter                (float input);

  private:

    float             coefficients          [7];            /* filter coefficients calculated by MATLAB tool */
    float             w                     [3];            /* memory for filtering calculations */
    ringbuffer<float> buffer_in;                            /* ring bugger of previous input */
    ringbuffer<float> buffer_out;                           /* ring bugger of previous outputs */
};


#endif //__BIQUAD_H_
