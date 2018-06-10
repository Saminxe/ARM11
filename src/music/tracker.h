#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define A 440 // A pitch standard
#define FREQUENCY_RATIO 1.05946309436 // twelfth root of 2 for equal temperament
#define SAMPLE_RATE 44100 // sample rate in Hz

typedef enum {
  SQUARE, SAW, TRIANGLE, SINE, NOISE
} Shape;

typedef struct { // Dynamics Envelople (only supports non-curved envelopes rip)
  uint16_t atk; // Attack in ms
  uint16_t dec; // Decay in ms
  uint16_t sus; // Sus in %
  uint16_t rel; // Release in ms
} ADSR;

typedef struct {
  ADSR envelope;
  Shape waveshape;
  uint8_t mix; // volume mix
  int16_t octave;
  int32_t detune; // detune in +/- cents (100 cents = tone)
} Osc;

typedef struct {
  Osc osc1;
  Osc osc2;
  Osc osc3;
} InstrParams;

typedef struct {
  uint8_t pitch;
  uint8_t velocity;
  uint16_t duration;
} Note;

typedef struct {
  uint64_t frame_count;
  int voice_count;
  Note voices[];
} Frame;

int16_t *instrument(InstrParams parameters, int note, int duration, uint8_t velocity);
