#include "music.h"
#include <stdlib.h>
#include <stdio.h>

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

enum notes{A1, B1, C1, D1, E1, F1, G1, A2, B2, C2, D2, E2, F2, G2, A3};

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
    prob.right3 = 5;
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
      prob.left1 = 5;
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
      prob.right4 = 0;
      prob.left1 = 5;
      prob.left2 = 27;
      prob.left3 = 32;
      prob.left4 = 48;
      prob.right1 = 53;
      prob.right2 = 75;
      prob.right3 = 80;
      break;
    case (F2):
      prob.right3 = prob.right4 = 0;
      prob.left1 = 5;
      prob.left2 = 28;
      prob.left3 = 33;
      prob.left4 = 52;
      prob.right1 = 57;
      prob.right2 = 80;
      break;
    case (G2):
      prob.right2 = prob.right3 = prob.right4 = 0;
      prob.left1 = 5;
      prob.left2 = 43;
      prob.left3 = 48;
      prob.left4 = 75;
      prob.right1 = 5;
    case (A3):
      prob.right1 = prob.right2 = prob.right3 = prob.right4 = 0;
      prob.left1 = 5;
      prob.left2 = 45;
      prob.left3 = 50;
      prob.left4 = 80;
  }
}

int chooseNext() {
  //randomly chooses a move based off of the probabilities
  int num = rand() % 101;
  if (num > 0 && num <= prob.left1) {
    return -1;
  } else  if (num > prob.left1 && num <= prob.left2) {
    return -2;
  } else  if (num > prob.left2 && num <= prob.left3) {
    return -3;
  } else  if (num > prob.left3 && num <= prob.left4) {
    return -4;
  } else  if (num > prob.left4 && num <= prob.right1) {
    return 1;
  } else  if (num > prob.right1 && num <= prob.right2) {
    return 2;
  } else  if (num > prob.right2 && num <= prob.right3) {
    return 3;
  } else  if (num > prob.right3 && num <= prob.right4) {
    return 4;
  } else  if (num > prob.right4 && num <= prob.same) {
    return 0;
  }
}

void shift(enum notes note) {
  note += chooseNext();
}
//applies the shifted vlaue to the notes

void writeToFile(enum notes note) {
  FILE *f = fopen("notes.txt", "w");
  if (f == NULL)
  {
    printf("Error opening file!\n");
    exit(1);
  }
  fprintf(f, "%i\n", note);
  fclose(f);
}

int main(int argc, char **argv) {
  //TODO: turn into notes
  int num = rand() % 201;
  for (int i = 0; i < num; ++i) {
    writeToFile(note);
    setProbability(note);
    shift(note);
  }
}