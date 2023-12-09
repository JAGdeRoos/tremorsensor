/******************************************************************************
 * Project    : Tremor Sensor Inator
 * Author     : Jochem de Roos
 *
 * Description: Implementation file for main processing
******************************************************************************/

#include <arduinoFFT.h>
#include <Arduino_LSM9DS1.h>

#include "fast_fourier.h"
#include "ringbuffer.h"
#include "PE1MEW_Timer.h"
#include "butterworth.h"

#define DEBUG 0                 // for testing with a custom sine wave.

#define MEASUREMENT_PERIOD 20   // measure at 50 Hz
#if DEBUG
#define TEST_PERIOD 5           
#endif
float constants[28] = {1, 0, -1, 1, -0.553571802689660552587724851036909967661, 0.647175086870397153759881803125608712435, 0.374173203914198171382565760723082348704,
                       1, 0, -1, 1, -1.704384872779503456996508248266763985157, 0.841449820765228451158179723279317840934, 0.374173203914198171382565760723082348704,
                       1, 0, -1, 1, -1.330150365892840635950733485515229403973, 0.528282274914411265065439238242106512189, 0.325424490465780646974991441311431117356,
                       1, 0, -1, 1, -0.746363977154120283863392160128569230437, 0.318615410428532364051079639466479420662, 0.318615410428532364051079639466479420662
                      };

#if DEBUG
int sineA[] =                   
{ 128 , 150 , 171 , 191 , 209 , 225 ,
  238 , 247 , 253 , 255 , 253 , 247 ,
  238 , 225 , 209 , 191 , 171 , 150 ,
  128 , 105 ,  84 ,  64 ,  46 ,  30 ,
   17 ,   8 ,   2 ,   0 ,   2 ,   8 ,
   17 ,  30 ,  46 ,  64 ,  84 , 105 ,
  127
};
#endif

int counter = 0;
float value1 = 0;

butterworth butterworth_x(constants);    //identical, we want to check for all three directions of motion (watch could be rotated/backwards without affecting output)
butterworth butterworth_y(constants);
butterworth butterworth_z(constants);

PE1MEW_Timer t2;
#if DEBUG
PE1MEW_Timer t3;
#endif

float x, y, z = 0.0;

double frequency_tremor[3] = {0.0};
double amplitude_tremor[3] = {0.0};

fast_fourier fft_x(50,'x',&frequency_tremor[0],&amplitude_tremor[0]); //50Hz is frequency at which you're measuring
fast_fourier fft_y(50,'y',&frequency_tremor[1],&amplitude_tremor[1]);
fast_fourier fft_z(50,'z',&frequency_tremor[2],&amplitude_tremor[2]);

void setup() {
  Serial.begin(9600);
  delay(8000);
  t2.setExpiry(MEASUREMENT_PERIOD);
#if DEBUG
  t3.setExpiry(TEST_PERIOD);    
#endif
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void loop() {
  
  if (t2.getExpired()) {           
    if (IMU.accelerationAvailable())
    {
      IMU.readAcceleration(x, y, z);
    }
    //fft_x.Fft_Input(butterworth_x.filter((value1*0.01)+x)); // for testing with t3 sine
    fft_x.input (butterworth_x.filter(x));
    fft_y.input (butterworth_y.filter(y));
    fft_z.input (butterworth_z.filter(z));

    // output for user, only the axis with the highest amplitude will be displayed
    if (fft_x.get_amplitude() > fft_y.get_amplitude())
    {
      if (fft_x.get_amplitude() > fft_z.get_amplitude()) fft_x.print_tremor();
      else fft_y.print_tremor();
    }
    else{
      if (fft_y.get_amplitude() > fft_z.get_amplitude()) fft_y.print_tremor();
      else fft_z.print_tremor();
    }

    if(!t2.setExpiry(MEASUREMENT_PERIOD)){
      delay           (MEASUREMENT_PERIOD);
      t2.setExpiry    (MEASUREMENT_PERIOD);
    }
#if DEBUG
    if (t3.getExpired()) {           

    value1 = ((float)sineA[counter] - 128.0) / 128.0;
    counter++;
    if (counter > 36) {
      counter = 0;
    }
    t3.setExpiry(TEST_PERIOD);
#endif
  }
}
