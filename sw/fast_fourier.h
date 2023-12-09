/******************************************************************************
 * Project    : Tremor Sensor Inator
 * Author     : Jochem de Roos
 *
 * Description: Header file for the fast fourier transform class. The class
 * also performs tremor detection using a custom algorithm which searches for 
 * the most consistent tremor over the past 2 minutes.
******************************************************************************/

#ifndef __FAST_FOURIER_H_
#define __FAST_FOURIER_H_

#include <arduinoFFT.h>
#include <stdint.h>

#define NUM_SAMPLES 512


// computes fourier and also performs tremor detection kinda
// takes the filtered data and transforms it to the frequency domain every 10 seconds
// then the class tries to find the tremor in the frequency domain
class fast_fourier
{
  public:
    /*! @brief The standard constructor for the fast fourier transform class */
    fast_fourier                  (int _sampling_frequency, char _input_axis, double* _frequency, double* _amplitude) : 
        count                     (0), 
        axis                      (_input_axis), 
        vReal                     {0.0}, 
        vImag                     {0.0}, 
        prev_spectrum             {0.0}, 
        diff_spectrum             {0.0}, 
        active_spectrum           {false},
        sampling_frequency        (_sampling_frequency), 
        threshold                 (0.2), 
        decay                     (0.90), 
        FFT                       (arduinoFFT()),
        frequency_tremor          (_frequency), 
        amplitude_tremor          (_amplitude)
        {}

    /*! @brief The input function takes the filtered values from thhe butterworth filter and saves it into the vReal array.
     * After being called 512 times (and the araray is filled), it calls the transform function and resets count
     * @param in : butterworth filtered IMU axis input data */
    void        input             (float in);

    /*! @brief prints out the latest estimate of the tremor frequency and relative amplitude. 
     * @note : This function can be called at all times. */
    void        print_tremor      ();

    /*! @brief Getter for the amplitude of a tremor. 
     * @return : amplitude of the tremor of the current axis */
    double      get_amplitude     (){return *amplitude_tremor;};

    

  private:
    /*! @brief Once 512 data samples have been collected, this function performs the fast fourier transform using the arduinoFFT library
     * First the data is windowed, then it computes the fft, lastly it converts the fft values into a magnitude.
     * At the end of this, the magnitudes of each frequency component compared to the overall signal are stored in vReal.
     * Then the find_tremor function is called */
    void        transform         ();

    /*! @brief Uses the magnitude of the frequency components stored in vReal to find the tremor.
     * @note : The algorithm uses only the lower half of the samples to cut out the alias in the top half of transform data 
     * frequency components are from 0 to 25 Hz, algorithm looks for most consistent active frequency component*/
    void        find_tremor       ();

    /*! @brief Prints out the transformed frequency spectrum */
    void        print_transform   ();
    
    int         count;                                      /* keep track of number of samples before transform */
    char        axis;                                       /* To indicate what axis is being evaluated */
    double      vReal             [NUM_SAMPLES];            /* Hold the real data values, of size NUM_SAMPLES (512), with 50Hz sampling freq this results in a conversion every 10 seconds */
    double      vImag             [NUM_SAMPLES];            /* Hold the imaginary data values */
    double      prev_spectrum     [NUM_SAMPLES/2];          /* array of previous spectrum */
    double      diff_spectrum     [NUM_SAMPLES/2];          /* array of the difference between the current and previous frequency spectrum */
    bool        active_spectrum   [NUM_SAMPLES/2];          /* array of booleans for active/inactive frequency components */
    int         sampling_frequency;                         /* sampling frequency */
    double      threshold;                                  /* to determine which frequency components are active, and which are inactive */
    double      decay;                                      /* determines how far back the consistency algorithm looks */
    arduinoFFT  FFT;                                        /* arduinoFFT class instantiation which handles fft */
    double*     frequency_tremor;                           /* pointer to final tremor frequency */ 
    double*     amplitude_tremor;                           /* pointer to final tremor amplitude */


};

#endif //__FAST_FOURIER_H_
