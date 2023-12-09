/******************************************************************************
 * Project    : Tremor Sensor Inator
 * Author     : Jochem de Roos
 *
 * Description: Implementation file for the fast fourier class
******************************************************************************/

#include "fast_fourier.h"

void fast_fourier::input(float in) {
  vReal[count] = in;
  vImag[count] = 0.0;
  count++;

  if (count == NUM_SAMPLES - 1) {
    transform();
    count = 0;
  }
}



void fast_fourier::transform() {
  FFT.Windowing(vReal, NUM_SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Windowing */
  FFT.Compute(vReal, vImag, NUM_SAMPLES, FFT_FORWARD);                  /* Compute FFT */
  FFT.ComplexToMagnitude(vReal, vImag, NUM_SAMPLES);                    /* Compute magnitudes */
  find_tremor();
}


void fast_fourier::find_tremor() {

  for (int i = 0; i < NUM_SAMPLES / 2; i++) {
    diff_spectrum[i] = diff_spectrum[i] * decay;
    diff_spectrum[i] += abs(prev_spectrum[i] - vReal[i]) / ((abs(vReal[i]) + abs(prev_spectrum[i])) / 2);
    if (vReal[i] > threshold) {
      active_spectrum[i] = true;
    }
    else {
      active_spectrum[i] = false;
    }
    prev_spectrum[i] = vReal[i];
  }

  // find min diff and check active_spectrum.
  int index = 0;
  double lowest_diff = 100000.0; // make sure it is greatest at the start.

  for (int i = 0; i < (NUM_SAMPLES / 2) - 1; i++) {
    if (active_spectrum[i]) {
      if (diff_spectrum[i] < lowest_diff) {
        index = i;
        lowest_diff = diff_spectrum[i];
      }
    }
  }
  *frequency_tremor = ((float(index) * sampling_frequency) / NUM_SAMPLES);
  *amplitude_tremor = vReal[index];
  
  //print_tremor();
}

void fast_fourier::print_transform() {
  Serial.print  (axis);
  Serial.print  ("-axis ");
  Serial.println("frequency domain is:");
  double F_scale = 0;
  for (int i = 0; i < NUM_SAMPLES / 2; i++) { // NUM_SAMPLES/2 to remove the ailias.
    F_scale = ((i * 1.0 * sampling_frequency) / NUM_SAMPLES);
    Serial.print  (vReal[i]);
    Serial.print  (", ");
    if (i % 10 == 0) {
      Serial.print  ("||");
      Serial.print  (F_scale);
      Serial.println(" hz");
    }
  }
  Serial.println("_________________________");
}

void fast_fourier::print_tremor() {
  Serial.print      (axis);
  Serial.println    ("-axis:");
  Serial.print      ("tremor frequency is: ");
  Serial.println    (*frequency_tremor, 4);
  Serial.print      ("tremor amplitude is: ");
  Serial.println    (*amplitude_tremor, 4);
  Serial.println    (" ");
}
