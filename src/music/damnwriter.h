#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <stdint.h>

// damn, you found my naming scheme
// get it? DAMN!!

typedef struct _DIYA {
  char damn[4];
  uint32_t instrument_count;
  uint8_t tempo;
  uint8_t fpb;
} Header;

typedef struct _AKARSHA {
  uint16_t attack;
  uint16_t decay;
  uint16_t sustain;
  uint16_t release;
  uint8_t shape;
  uint8_t mix;
  uint16_t offset;
  uint32_t detune;
} OscillatorParameters;

typedef struct _MIN {
  OscillatorParameters osc1;
  OscillatorParameters osc2;
  OscillatorParameters osc3;
} InstrumentParameters;

typedef struct _NOELLE {
  uint8_t pitch;
  uint8_t velocity;
  uint16_t duration;
} Note;
