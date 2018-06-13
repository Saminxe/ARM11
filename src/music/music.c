#include "music.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

//This is for C Major

struct probability {
  int left1;
  int left2;
  int left3;
  int left4;
  int right1;
  int right2;
  int right3;
  int right4;
  int same;
} prob;

enum notes{A1 = 0x2D, B1 = 0x2F, C1 = 0x30, D1 = 0x32, E1 = 0x34, F1 = 0x35, G1 = 0x37, A2 = 0x39, B2 = 0x3B, C2 = 0x3C, D2 = 0x3E, E2 = 0x40, F2 = 0x42, G2 = 0x43, A3 = 0x45};


//Sets the probabilities that are based off our personal musical experience
void setProbability(enum notes note) {
  //probabilities are cumulative for our function to work
  prob.same = 100;
  if (note > 3 && note < 11 && note !=6 && note != 9) {
    //notes outside this range could go out of bounds or is G1/C2
    prob.left1 = 3;
    prob.left2 = 23;
    prob.left3 = 25;
    prob.left4 = 40;
    prob.right1 = 43;
    prob.right2 = 63;
    prob.right3 = 65;
    prob.right4 = 80;
  } else if (note == 6) {
    //G1 is a special case in C Major
    prob.left1 = 3;
    prob.left2 = 23;
    prob.left3 = 25;
    prob.left4 = 40;
    prob.right1 = 43;
    prob.right2 = 53;
    prob.right3 = 78;
    prob.right4 = 80;
  } else if (note == 9) {
    //C2 is also special but in the reverse manner to G1
    prob.left1 = 3;
    prob.left2 = 13;
    prob.left3 = 38;
    prob.left4 = 40;
    prob.right1 = 43;
    prob.right2 = 63;
    prob.right3 = 68;
    prob.right4 = 80;
  }
  //I understand nested if statements would use less repeated code but it would also look disgusting

  switch(note) {
    case (A1):
      prob.left1 = prob.left2 = prob.left3 = prob.left4 = 0;
      prob.right1 = 5;
      prob.right2 = 45;
      prob.right3 = 50;
      prob.right4 = 80;
      break;
    case (B1):
      prob.left2 = prob.left3 = prob.left4 = 0;
      prob.right1 = 5;
      prob.right2 = 43;
      prob.right3 = 48;
      prob.right4 = 75;
      prob.left1 = 80;
      break;
    case (C1):
      prob.left3 = prob.left4 = 0;
      prob.right1 = 5;
      prob.right2 = 28;
      prob.right3 = 33;
      prob.right4 = 52;
      prob.left1 = 57;
      prob.left2 = 80;
      break;
    case (D1):
      prob.left4 = 0;
      prob.right1 = 5;
      prob.right2 = 27;
      prob.right3 = 32;
      prob.right4 = 48;
      prob.left1 = 53;
      prob.left2 = 75;
      prob.left3 = 80;
      break;
    case (E1):
      break;
    case (F1):
      break;
    case (G1):
      break;
    case (A2):
      break;
    case (B2):
      break;
    case (C2):
      break;
    case (D2):
      break;
    case (E2):
      prob.left1 = 5;
      prob.left2 = 27;
      prob.left3 = 32;
      prob.left4 = 48;
      prob.right1 = 53;
      prob.right2 = 75;
      prob.right3 = 80;
      prob.right4 = 80;
      break;
    case (F2):
      prob.left1 = 5;
      prob.left2 = 28;
      prob.left3 = 33;
      prob.left4 = 52;
      prob.right1 = 57;
      prob.right2 = 80;
      prob.right3 = prob.right4 = 80;
      break;
    case (G2):
      prob.left1 = 5;
      prob.left2 = 43;
      prob.left3 = 48;
      prob.left4 = 75;
      prob.right1 = 80;
      prob.right2 = prob.right3 = prob.right4 = 80;
      break;
    case (A3):
      prob.left1 = 5;
      prob.left2 = 45;
      prob.left3 = 50;
      prob.left4 = 80;
      prob.right1 = prob.right2 = prob.right3 = prob.right4 = 80;
      break;
  }
}

//Randomly chooses a move based off of the probabilities
int chooseNext() {

  int num = rand() % 101;
  printf("%u\n", num);
  if (num > 0 && num <= prob.left1) {
    return -2;
  } else  if (num > prob.left1 && num <= prob.left2) {
    return -4;
  } else  if (num > prob.left2 && num <= prob.left3) {
    return -6;
  } else  if (num > prob.left3 && num <= prob.left4) {
    return -8;
  } else  if (num > prob.left4 && num <= prob.right1) {
    return 2;
  } else  if (num > prob.right1 && num <= prob.right2) {
    return 4;
  } else  if (num > prob.right2 && num <= prob.right3) {
    return 6;
  } else  if (num > prob.right3 && num <= prob.right4) {
    return 8;
  } else  if (num > prob.right4 && num <= prob.same) {
    return 0;
  }
  return 0;

}

//Applies the shifted vlaue to the notes
uint8_t shift(enum notes note) {
  note += chooseNext();
  return note;
}


//Converts the input String to it's numerical representation
uint8_t enumToInt(char *note) {
  if (!strcmp(note, "A1")) {
    return 0x2D;
  } else if(!strcmp(note, "B1")) {
    return 0x2F;
  } else if(!strcmp(note, "C1")) {
    return 0x30;
  } else if(!strcmp(note, "D1")) {
    return 0x32;
  } else if(!strcmp(note, "E1")) {
    return 0x34;
  } else if(!strcmp(note, "F1")) {
    return 0x35;
  } else if(!strcmp(note, "G1")) {
    return 0x37;
  } else if(!strcmp(note, "A2")) {
    return 0x39;
  } else if(!strcmp(note, "B2")) {
    return 0x3B;
  } else if(!strcmp(note, "C2")) {
    return 0x3C;
  } else if(!strcmp(note, "D2")) {
    return 0x3E;
  } else if(!strcmp(note, "E2")) {
    return 0x40;
  } else if(!strcmp(note, "F2")) {
    return 0x42;
  } else if(!strcmp(note, "G2")) {
    return 0x43;
  } else if(!strcmp(note, "A3")) {
    return 0x45;
  }
  printf("Not a valid note you schmuk\n");
  exit(EXIT_FAILURE);
}


int main(void) {
  srand(time(NULL));
  FILE *f = fopen("notes.damn", "wb");
  if (f == NULL)
  {
    printf("Error opening file!\n");
    exit(1);
  }

  //Here is where we create the DAMN header so that the files can be read and understood
  char damn[] = {'D','A','M','N'};
  uint32_t no_instr = 2;
  uint8_t tempo = 0x78;
  uint8_t fpb = 0x04;
  uint16_t non = 0x0;
  uint16_t attack = 0x5;
  uint16_t decay = 0x5;
  uint16_t sustain = 0x32;
  uint16_t release = 0x5;
  uint8_t waveshape = 0x0;
  uint8_t waveshape2 = 0x1;
  uint8_t waveshape3 = 0x3;
  uint8_t oscillator_volume = 0x64;
  uint16_t offset = 0x0;
  uint32_t detune = 0x0;
  uint64_t osc = 0x0;

  fwrite(damn, 1, 4, f);
  fwrite(&no_instr, 1, 4, f);
  fwrite(&tempo, 1, 1, f);
  fwrite(&fpb, 1, 1, f);
  fwrite(&non, 1, 2, f);
  fwrite(&non, 1, 2, f);
  fwrite(&non, 1, 2, f);
  fwrite(&attack, 1, 2, f);
  fwrite(&decay, 1, 2, f);
  fwrite(&sustain, 1, 2, f);
  fwrite(&release, 1, 2, f);
  fwrite(&waveshape, 1, 1, f);
  fwrite(&oscillator_volume, 1, 1, f);
  fwrite(&offset, 1, 2, f);
  fwrite(&detune, 1, 4, f);
  for (int i = 0; i < 8; i++) fwrite(&osc, 1, 4, f);
  fwrite(&attack, 1, 2, f);
  fwrite(&decay, 1, 2, f);
  fwrite(&sustain, 1, 2, f);
  fwrite(&release, 1, 2, f);
  fwrite(&waveshape2, 1, 1, f);
  fwrite(&oscillator_volume, 1, 1, f);
  fwrite(&offset, 1, 2, f);
  fwrite(&detune, 1, 4, f);
  for (int i = 0; i < 8; i++) fwrite(&osc, 1, 4, f);
  fwrite(&attack, 1, 2, f);
  fwrite(&decay, 1, 2, f);
  fwrite(&sustain, 1, 2, f);
  fwrite(&release, 1, 2, f);
  fwrite(&waveshape3, 1, 1, f);
  fwrite(&oscillator_volume, 1, 1, f);
  fwrite(&offset, 1, 2, f);
  fwrite(&detune, 1, 4, f);
  for (int i = 0; i < 8; i++) fwrite(&osc, 1, 4, f);

  srand(time(NULL));
  char *note = malloc(sizeof(char*));
  scanf("%s", note);
  uint8_t *muse;
  uint8_t temp = enumToInt(note);
  uint8_t velocity = 0x28;
  for (int i = 0; i < 6000; ++i) {
    muse = &temp;
    fwrite(muse, 1, 1, f);
    fwrite(&velocity, 1, 1, f);
    fwrite(&non, 1, 2, f);
    setProbability(temp);
    temp = shift(temp) % 74 + 23;
  }
  free(note);
  fclose(f);
  return 0;
}