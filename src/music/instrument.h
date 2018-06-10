#ifndef <stdio.h>
#include <stdio.h>
#endif
#ifndef <stdlib.h>
#include <stdlib.h>
#endif
#ifndef <stdint.h>
#include <stdint.h>
#endif
#ifndef <math.h>
#include <math.h>
#endif
#ifndef <time.h>
#include <time.h>
#endif

#define A 440 // A pitch standard
#define FREQUENCY_RATIO 1.05946309436 // twelfth root of 2 for equal temperament
#define SAMPLE_RATE 44100 // sample rate in Hz

typedef enum {
  SQUARE, SAW, TRIANGLE, NOISE, SINE
} Shape;

typedef struct { // Dynamics Envelople (only supports non-curved envelopes rip)
  int atk; // Attack in ms
  int dec; // Decay in ms
  int sus; // Sus in %
  int rel; // Release in ms
} ADSR;

typedef struct {
  ADSR envelope;
  int detune; // detune in +/- cents (100 cents = tone)
  int octave;
  Shape waveshape;
} Osc;

typedef struct {
  double osc1_mix; // volume mix
  Osc osc1;
  double osc2_mix; // volume mix
  Osc osc2;
  double osc3_mix; // volume mix
  Osc osc3;
} InstrParams;

int16_t *instrument(InstrParams parameters, int note, int duration);
