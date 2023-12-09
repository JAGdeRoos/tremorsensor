/******************************************************************************
 * Project    : Tremor Sensor Inator
 * Author     : Jochem de Roos
 *
 * Description: Implementation file for butterworth filter class
******************************************************************************/

#include "butterworth.h"

butterworth::butterworth(float c[28])
{
  float c1[7];
  float c2[7];
  float c3[7];
  float c4[7];

  for (int i = 0; i < 7; i++) {
    c1[i] = c[i];
    c2[i] = c[i + 7];
    c3[i] = c[i + 14];
    c4[i] = c[i + 21];
  }
  
  filter1.set_coefficients(c1);
  filter2.set_coefficients(c2);
  filter3.set_coefficients(c3);
  filter4.set_coefficients(c4);

}

float butterworth::filter(float input)
{
  float output = filter1.filter(input);
  output = filter2.filter(output);
  output = filter3.filter(output);
  output = filter4.filter(output);
  
  return output;
}

void butterworth::print_biquad_coefficients()
{
  Serial.println("the constants for the different biquad sections");
  filter1.print_coefficients();
  filter2.print_coefficients();
  filter3.print_coefficients();
  filter4.print_coefficients();
}
