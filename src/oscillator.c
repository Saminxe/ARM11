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
  int atk_len = (envelope.atk * SAMPLE_RATE) / 1000;
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

  // Calculates the frequency of the note to be played.
  double freq = (A) * pow(FREQUENCY_RATIO, (double) (note - 69));
  printf("note = %fHz\n", freq);
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
      wave_ptr++;
    }
  } else if (wave == SAW) {
    while (wave_ptr < wave_length) {
      double envelope_ratio = (double) *(waveform + wave_ptr) / INT16_MAX;
      int timer = wave_ptr % period;
      int value = ((double) (timer * 2) / period) * INT16_MAX;
      if (value >= INT16_MAX) {
        value = value - (2 * INT16_MAX);
      }
      printf("%d\n", value);
      *(waveform + wave_ptr) = (int16_t) (envelope_ratio * value);
      wave_ptr++;
    }
  }


  return waveform;
}

int main(int argc, char const *argv[]) {
  ADSR envelope = {10, 10, 50, 10};
  FILE *wavey = fopen("waveform", "wb");
  int16_t *waveform = play(envelope, SAW, 69, 50);
  int wavlen = (50 * SAMPLE_RATE) / 1000;
  fwrite(waveform, sizeof(int16_t), wavlen, wavey);
  fclose(wavey);
  return 0;
}
