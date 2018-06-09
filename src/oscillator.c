#include "oscillator.h"
#include "music.h"

/***
Envelope: Denotes the volume envelople
Waveshape: Denotes the type of wave to produce
Note: The note to play (C0 = 0, B9 = 131)
Duration: The length of the note in ms
This is a 16-bit depth oscillator.
***/

int16_t *play(ADSR envelope, Shape wave, int note, int duration)
{
  // Initializes the waveform to be returned.
  int wave_length = (duration * SAMPLE_RATE) / 1000;
  int16_t *waveform = calloc(wave_length, sizeof(int16_t));
  int wave_ptr = 0;

  // Calculates the length of envelope sections in frames
  int akt_len = (envelope.atk * SAMPLE_RATE) / 1000;
  int dec_len = (envelope.dec * SAMPLE_RATE) / 1000;
  int rel_len = (envelope.rel * SAMPLE_RATE) / 1000;
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
  int16_t sus_amplitude = (int16_t) (((double) envelope.sus / 100) * INT16_MAX)
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

  //Release profile
  for (int i = 0; i < rel_len; i++) {
    if (wave_ptr < wave_length) {
      *(waveform + wave_ptr) = sus_amplitude - (int16_t) (((double) i / rel_len) * sus_amplitude);
      wave_ptr++;
    }
  }

  return waveform;
}

int main(int argc, char const *argv[]) {
  ADSR = {10, 10, 50, 10};
  FILE *wavey = fopen("waveform", "w");

  return 0;
}
