/******************************************************************************
 * Project    : Tremor Sensor Inator
 * Author     : Jochem de Roos
 *
 * Description: Header file for butterworth filter. The 8th order butterworth 
 * filter is made up of 4 second order biquad filters each defining a part of 
 * the butterworth curve, by passing the data through each biquad consecutively.
 * 
 * Filter coefficients were obtained from MATLAB by using the filter designer 
 * tool.
******************************************************************************/

#ifndef __BUTTERWORTH_H_
#define __BUTTERWORTH_H_

#include <stdint.h>
#include <Arduino.h>  /* for millis() */

#include "biquad.h"   /* makes up the butterworth curve */


class butterworth
{
  public:
    /*! @brief The constructor for the butterworth filter.
     * Splits up 28 coefficients into 4 arrays of 7 coefficients, then sets these 
     * coefficients in each of the biquad filters that make up this butterworth filter
     * @param c[28] : the 28 coefficient constants as defined by MATLAB filter designer tool */
    butterworth                     (float c[28]);

    /*! @brief The filter function for the butterworth filter.
     * Calls the filter function from a biquad filter, and places the return value as the input for the next biquad filter
     * @param input : IMU axis input data
     * @return : butterworth filtered input data */
    float filter                    (float input);

    /*! @brief The pint function for the butterworth filter.
     * Calls the print functions of each biquad filter */
    void  print_biquad_coefficients ();

  private:
    biquad filter1;                 /* 1st biquad filter to make up the butterworth curve */
    biquad filter2;                 /* 2nd biquad filter to make up the butterworth curve */
    biquad filter3;                 /* 3rd biquad filter to make up the butterworth curve */
    biquad filter4;                 /* 4th biquad filter to make up the butterworth curve */
};


#endif //__BW_FILTER_H_
