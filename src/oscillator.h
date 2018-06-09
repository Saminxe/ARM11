#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
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
