#include "tracker.h"

/***
Introducing: The DAMN tracker!!!
DAMN: Digitally Approximated Music using NO-L

Reads binary .damn format and synthesizes it using the NO-L 3xOsc Synth!!!

DESCRIPTION of .DAMN FORMAT:
  HEADER:
    0x00 - 0x03: 4 bytes saying "DAMN"
    0x04 - 0x07: 4 bytes for the number of instruments/tracking channels (max 16, might change)
    0x08 - 0x0F: Metadata:
      0x08: Tempo
      0x09: Frames per beat (normally 32; demisemiquaver is minimum quantization)
      0x0A: Other stuff that might be useful later, like a gun that shoots a smaller gun that shoots a knife which explodes on impact
    0x10 onwards: (no. of instruments * 8) bytes for ADSR and waveshape information of the instruments:
      OFFSET:
        BITS 00-15: Attack length in ms
        BITS 16-31: Decay length in ms
        BITS 32-39: Sustain level in %
        BITS 40-55: Release length in ms
        BITS 56-64: Type of wave

  DATA:
    Each frame is (no. of instruments * 4) bytes large:
    FRAME SECTION:
      0x1: Note - 0 = C0, 131 = B9, 255 = STOP
      0x2: Dynamic mix of the note, 0 = nonexistent, 255 = max overdrive!!!
      0x3 - 0x4: Sustain/Pulse duration, 0 = sustain, any other number represents pulse duration in frames.
        The sustain note stops when a STOP note appears on the channel again.
    After the expected number of instrument notes is read, the next 4 bytes is interpreted as a note in a new frame.
    The reading continues until the file ends OR a -1 frame is found (ie 0xFFFFFFFF)

  OUTPUT:
    Since this is 'approximated' music, it's only mono and will write to a wave file.
***/
