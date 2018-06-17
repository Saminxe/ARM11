#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <assert.h>

/* I hate you. I hate you more than the stringy things on bananas. */

typedef struct _SNAKEY {
  char damn[4];
  uint32_t instrument_count;
  uint8_t tempo;
  uint8_t fpb;
} Header;

typedef struct _HAYDEN {
  uint16_t attack;
  uint16_t decay;
  uint16_t sustain;
  uint16_t release;
  uint8_t wave;
  uint8_t mix;
  int16_t octave;
  int32_t detune;
} Oscillator;

typedef struct _JUNSEO {
  Oscillator oscillators[3];
} Instrument;

typedef struct _CHRYSSA {
  uint8_t pitch;
  uint8_t velocity;
  uint16_t sustain;
} Note;

typedef struct _LIZ {
  Note *notes;
} Frame;
