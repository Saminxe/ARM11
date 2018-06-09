#include "instrument.h"

/***
INTRODUCING: THE NO-L SYNTHESIZER
NO-L for (definitely) Not Logical Synthesizer

BEAUTIFUL FEATURES OF NO-L:
 - Produce square, saw, triangle and sine waves, plus noise!!!
 - Support for dynamic enveloping!!!
 - Instruments support multiple oscillators at different pitches!!!
 - Writes directly to PCM!!!
 - Change the tuning so u can be electro-baroque af!!!
 - Supports any sample rate (don't make it less than 40000Hz!!!)!!!
 - Built in constant representing the twelfth root of two!!!

~~~ Choose NO-L for your next music project!!! ~~~
***/

int max(int a, int b)
{
  return (a < b) ? b : a;
}

int min(int a, int b)
{
  return (a < b) ? a : b;
}

/***
Envelope: Denotes the volume envelople
Waveshape: Denotes the type of wave to produce
Frequency: The frequency of the note in Hz
Duration: The length of the note in ms
This is a 16-bit depth oscillator.
***/

int16_t *osc(ADSR envelope, Shape wave, double freq, int duration)
{
  // Calculates the length of envelope sections in frames
  int atk_len = (envelope.atk * SAMPLE_RATE) / 1000;
  int dec_len = (envelope.dec * SAMPLE_RATE) / 1000;
  int rel_len = (envelope.rel * SAMPLE_RATE) / 1000;

  // Initializes the waveform to be returned.
  uint32_t wave_length = ((duration * SAMPLE_RATE) / 1000) + rel_len;
  int16_t *waveform = calloc(wave_length, sizeof(int16_t));
  uint32_t wave_ptr = 0;

  int sus_len = wave_length - (atk_len + dec_len + rel_len);

  // Populates the waveform with the volume envelople
  // Attack profile
  for (int i = 0; i < atk_len; i++) {
    if (wave_ptr < wave_length) {
      *(waveform + wave_ptr) = (int16_t) (((double) i / atk_len) * INT16_MAX);
      wave_ptr++;
    }
  }

  // Decay profile
  int16_t sus_amplitude = (int16_t) (((double) envelope.sus / 100) * INT16_MAX);
  for (int i = 0; i < dec_len; i++) {
    if (wave_ptr < wave_length) {
      *(waveform + wave_ptr) = INT16_MAX - (int16_t) (((double) i / dec_len) * sus_amplitude);
      wave_ptr++;
    }
  }

  // Sustain profile
  for (int i = 0; i < sus_len; i++) {
    if (wave_ptr < wave_length) {
      *(waveform + wave_ptr) = sus_amplitude;
      wave_ptr++;
    }
  }

  // Release profile
  for (int i = 0; i < rel_len; i++) {
    if (wave_ptr < wave_length) {
      *(waveform + wave_ptr) = sus_amplitude - (int16_t) (((double) i / rel_len) * sus_amplitude);
      wave_ptr++;
    }
  }

  wave_ptr = 0; // Resets the pointer

  // Calculates the number of frames a single wave takes up.
  int period = (int) ((1.0 / freq) * SAMPLE_RATE);
  printf("period = %fms, %d frames\n", 1000.0 / freq, period);

  // Populates the waveform with the appropriate wave shape.
  // For square waves, the pulse width ratio is assumed to be 50%.
  if (wave == SQUARE) {
    while (wave_ptr < wave_length) {
      int16_t envelope = *(waveform + wave_ptr);
      if ((wave_ptr % period) < (period / 2)) { // we can change the PW here
        *(waveform + wave_ptr) = envelope;
      } else {
        *(waveform + wave_ptr) = - envelope;
      }
      printf("Frame %u: %d\n", wave_ptr, *(waveform + wave_ptr));
      wave_ptr++;
    }
  } else if (wave == SAW) {
    while (wave_ptr < wave_length) {
      double envelope_ratio = (double) *(waveform + wave_ptr) / INT16_MAX;
      int timer = wave_ptr % period;
      int16_t value = ((double) (timer * 2) / period) * INT16_MAX;
      if (value >= INT16_MAX) {
        value = value - (2 * INT16_MAX);
      }
      value = (int16_t) (envelope_ratio * value);
      *(waveform + wave_ptr) = value;
      printf("Frame %u: %d\n", wave_ptr, value);
      wave_ptr++;
    }
  } else if (wave == TRIANGLE) {
    while (wave_ptr < wave_length) {
      double envelope_ratio = (double) *(waveform + wave_ptr) / INT16_MAX;
      int timer = wave_ptr % period;
      int16_t value = ((double) (timer * 4) / period) * INT16_MAX;
      if (value >= INT16_MAX && value < 3 * INT16_MAX) {
        value = 2 * INT16_MAX - value;
      } else if (value >= 3 * INT16_MAX) {
        value = value - 4 * INT16_MAX;
      }
      value = (int16_t) (envelope_ratio * value);
      *(waveform + wave_ptr) = value;
      printf("Frame %u: %d\n", wave_ptr, value);
      wave_ptr++;
    }
  } else if (wave == NOISE) { // v bad implementation of noise, do not use
    srand(time(NULL));
    while (wave_ptr < wave_length) {
      double envelope_ratio = (double) *(waveform + wave_ptr) / INT16_MAX;
      int16_t value = (rand() % UINT16_MAX) - INT16_MAX;
      value = (int16_t) (envelope_ratio * value);
      *(waveform + wave_ptr) = value;
      printf("Frame %u: %d\n", wave_ptr, value);
      wave_ptr++;
    }
  } else if (wave == SINE) {
    while (wave_ptr < wave_length) {
      double envelope_ratio = (double) *(waveform + wave_ptr) / INT16_MAX;
      int16_t value = (int) (sin(((2 * M_PI) / period) * wave_ptr) * INT16_MAX);
      value = (int16_t) (envelope_ratio * value);
      *(waveform + wave_ptr) = value;
      printf("Frame %u: %d\n", wave_ptr, value);
      wave_ptr++;
    }
  }

  return waveform;
}

int16_t *scale(double scalar, int16_t *wave, int wave_size)
{
  for (int i = 0; i < wave_size; i++) {
    wave[i] = (int16_t) ((double) wave[i] * scalar);
  }
  return wave;
}

int16_t *superpose(int16_t *wave1, int size1, int16_t *wave2, int size2)
{
  if (size1 < size2) {
    for (int i = 0; i < size1; i++) {
      *(wave2 + i) = *(wave1 + i) + *(wave2 + i);
    }
    return wave2;
  } else {
    for (int i = 0; i < size2; i++) {
      *(wave1 + i) = *(wave1 + i) + *(wave2 + i);
    }
    return wave1;
  }
}

/***
Instruments consists of 3 oscillators.

They can be set to play at octave offsets from the main note, or be detuned
up to 1200 cents in each direction (+/-).

Each the mix of each oscillator can be controlled; if the mix is 0, it will not
be played.

Each oscillator's output is added to each other.

Post filters can then control the sound of the instrument.
***/
int16_t *instrument(InstrParams parameters, int note, int duration)
{
  int max_release = max(max(
    parameters.osc1.envelope.rel,
    parameters.osc2.envelope.rel),
    parameters.osc3.envelope.rel
  );
  int wave_length = ((duration + max_release) * SAMPLE_RATE) / 1000;
  int16_t *waveform = calloc(wave_length, sizeof(int16_t));
  int osc1_size = ((parameters.osc1.envelope.rel + duration) * SAMPLE_RATE) / 1000;
  int osc2_size = ((parameters.osc2.envelope.rel + duration) * SAMPLE_RATE) / 1000;
  int osc3_size = ((parameters.osc3.envelope.rel + duration) * SAMPLE_RATE) / 1000;
  double osc1_note =
    note + (parameters.osc1.detune / 100) + (12 * parameters.osc1.octave);
  double osc1_frequency = (A) * pow(FREQUENCY_RATIO, osc1_note - 69);
  double osc2_note =
    note + (parameters.osc2.detune / 100) + (12 * parameters.osc2.octave);
  double osc2_frequency = (A) * pow(FREQUENCY_RATIO, osc2_note - 69);
  double osc3_note =
    note + (parameters.osc3.detune / 100) + (12 * parameters.osc3.octave);
  double osc3_frequency = (A) * pow(FREQUENCY_RATIO, osc3_note - 69);
  int16_t *osc1_wave = osc(parameters.osc1.envelope, parameters.osc1.waveshape, osc1_frequency, duration);
  int16_t *osc2_wave = osc(parameters.osc2.envelope, parameters.osc2.waveshape, osc2_frequency, duration);
  int16_t *osc3_wave = osc(parameters.osc3.envelope, parameters.osc3.waveshape, osc3_frequency, duration);

  osc1_wave = scale(parameters.osc1_mix, osc1_wave, osc1_size);
  osc2_wave = scale(parameters.osc2_mix, osc2_wave, osc2_size);
  osc3_wave = scale(parameters.osc3_mix, osc3_wave, osc3_size);

  waveform = superpose(waveform, wave_length, osc1_wave, osc1_size);
  waveform = superpose(waveform, wave_length, osc2_wave, osc2_size);
  waveform = superpose(waveform, wave_length, osc3_wave, osc3_size);

  free(osc1_wave);
  free(osc2_wave);
  free(osc3_wave);

  return waveform;
}

int main(int argc, char const *argv[]) {
  ADSR envelope = {10, 10, 50, 10};
  Osc osc1 = {envelope, 0, 0, SQUARE};
  Osc osc2 = {envelope, 0, 0, SINE};
  Osc osc3 = {envelope, 0, 0, SAW};
  InstrParams instr = {0.4, osc1, 0.3, osc2, 0.3, osc3};
  FILE *wavey = fopen("waveform", "wb");
  int16_t *waveform = instrument(instr, 69, 1000);
  int wavlen = (1000 * SAMPLE_RATE) / 1000;
  fwrite(waveform, sizeof(int16_t), wavlen, wavey);
  free(waveform);
  fclose(wavey);
  return 0;
}
