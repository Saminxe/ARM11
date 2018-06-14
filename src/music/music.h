#ifndef ARM11_07_MUSIC_H
#define ARM11_07_MUSIC_H

#endif //ARM11_07_MUSIC_H

typedef struct { // using scientific pitch notation
  int pitch; // 0 = C -> 12 = B
  int octave; // from 0 - 9
} Note;

typedef struct _CHRP {
  int I;
  int II;
  int III;
  int IV;
  int V;
  int VI;
  int VII;
} ChordProbability;

typedef struct _JP {
  int subV;
  int subIV;
  int subIII;
  int subII;
  int zero;
  int addII;
  int addIII;
  int addIV;
  int addV;
} JumpProbabilities;

typedef enum _CHR {
  A = 57, As, B, C, Cs, D, Ds, E, F, Fs, G, Gs
} Chromatics;
