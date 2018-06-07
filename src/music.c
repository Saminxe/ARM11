#include "music.h"

//This is for C Major

typedef struct probability {
  double left1;
  double left2;
  double left3;
  double left4;
  double right1;
  double right2;
  double right3;
  double right4;
  double same;
};

enum notes{A1, B1, C1, D1, E1, F1, G1, A2, B2, C2, D2, E2, F2, G2, A3};

void setProbability(enum notes note) {
  struct probability *probabilityPtr, prob;
  probabilityPtr = &prob;
  probabilityPtr->same = 0.2;

  if (note > 3 && note < 11 && note !=6 && note != 9) {  //notes outside this range could go out of bounds or is G1/C2
    probabilityPtr->left1 = 0.025;
    probabilityPtr->left2 = 0.2;
    probabilityPtr->left3 = 0.025;
    probabilityPtr->left4 = 0.15;
    probabilityPtr->right1 = 0.025;
    probabilityPtr->right2 = 0.2;
    probabilityPtr->right3 = 0.025;
    probabilityPtr->right4 = 0.15;
  } else if (note == 6) {                            //G1 is a special case in C Major
    probabilityPtr->left1 = 0.025;
    probabilityPtr->left2 = 0.2;
    probabilityPtr->left3 = 0.025;
    probabilityPtr->left4 = 0.15;
    probabilityPtr->right1 = 0.025;
    probabilityPtr->right2 = 0.1;
    probabilityPtr->right3 = 0.25;
    probabilityPtr->right4 = 0.025;
  } else if (note == 9) {                            //C2 is also special but in the reverse manner to G1
    probabilityPtr->left1 = 0.025;
    probabilityPtr->left2 = 0.1;
    probabilityPtr->left3 = 0.25;
    probabilityPtr->left4 = 0.025;
    probabilityPtr->right1 = 0.025;
    probabilityPtr->right2 = 0.2;
    probabilityPtr->right3 = 0.025;
    probabilityPtr->right4 = 0.15;
  }
  //I understand nested if statements would use less repeated code but it would also look disgusting

  switch(note) {
    case (A1):
      probabilityPtr->left1 = probabilityPtr->left2 = probabilityPtr->left3 = probabilityPtr->left4 = 0;
      probabilityPtr->right1 = 0.05;
      probabilityPtr->right2 = 0.4;
      probabilityPtr->right3 = 0.05;
      probabilityPtr->right4 = 0.3;
      break;
    case (B1):
      probabilityPtr->left2 = probabilityPtr->left3 = probabilityPtr->left4 = 0;
      probabilityPtr->right1 = 0.05;
      probabilityPtr->right2 = 0.375;
      probabilityPtr->right3 = 0.05;
      probabilityPtr->right4 = 0.275;
      probabilityPtr->left1 = 0.05;
      break;
    case (C1):
      probabilityPtr->left3 = probabilityPtr->left4 = 0;
      probabilityPtr->right1 = 0.05;
      probabilityPtr->right2 = 0.23;
      probabilityPtr->right3 = 0.05;
      probabilityPtr->right4 = 0.19;
      probabilityPtr->left1 = 0.05;
      probabilityPtr->left2 = 0.23;
      break;
    case (D1):
      probabilityPtr->left4 = 0;
      probabilityPtr->right1 = 0.05;
      probabilityPtr->right2 = 0.21;
      probabilityPtr->right3 = 0.05;
      probabilityPtr->right4 = 0.16;
      probabilityPtr->left1 = 0.05;
      probabilityPtr->left2 = 0.21;
      probabilityPtr->left3 = 0.05;
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
      probabilityPtr->right4 = 0;
      probabilityPtr->left1 = 0.05;
      probabilityPtr->left2 = 0.21;
      probabilityPtr->left3 = 0.05;
      probabilityPtr->left4 = 0.16;
      probabilityPtr->right1 = 0.05;
      probabilityPtr->right2 = 0.21;
      probabilityPtr->right3 = 0.05;
      break;
    case (F2):
      probabilityPtr->right3 = probabilityPtr->right4 = 0;
      probabilityPtr->left1 = 0.05;
      probabilityPtr->left2 = 0.23;
      probabilityPtr->left3 = 0.05;
      probabilityPtr->left4 = 0.19;
      probabilityPtr->right1 = 0.05;
      probabilityPtr->right2 =0.23;
      break;
    case (G2):
      probabilityPtr->right2 = probabilityPtr->right3 = probabilityPtr->right4 = 0;
      probabilityPtr->left1 = 0.05;
      probabilityPtr->left2 = 0.375;
      probabilityPtr->left3 = 0.05;
      probabilityPtr->left4 = 0.275;
      probabilityPtr->right1 = 0.05;
    case (A3):
      probabilityPtr->right1 = probabilityPtr->right2 = probabilityPtr->right3 = probabilityPtr->right4 = 0;
      probabilityPtr->left1 = 0.05;
      probabilityPtr->left2 = 0.4;
      probabilityPtr->left3 = 0.05;
      probabilityPtr->left4 = 0.3;
  }

}