/******************************************************************************
 * Project    : Tremor Sensor Inator
 * Author     : Jochem de Roos
 *
 * Description: Implementation file for biquad filter
******************************************************************************/

#include "biquad.h"

float biquad::filter(float input)
{
  float output = 0;
  w[0] = input+(-1*coefficients[4])*w[1]+(-1*coefficients[5])*w[2];
  output = coefficients[0]*w[0] + coefficients[1]*w[1] + coefficients[2]*w[2];
  w[2] = w[1];
  w[1] = w[0]; 
  
  output = output * coefficients[6];
  return output;
}

void biquad::set_coefficients(float c[7])
{
  for (int i = 0; i < 7; i++) {
    coefficients[i] = c[i];
  }

}

void biquad::print_coefficients()
{
  for (int i = 0; i < 7; i++) {
    Serial.print(coefficients[i]);
    Serial.print(" , ");
  }
  Serial.println(" ");

}
