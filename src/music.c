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
}prob;

enum notes{A1, B1, C1, D1, E1, F1, G1, A2, B2, C2, D2, E2, F2, G2, A3};

void setProbability(enum notes note) {
  struct probability prob;
  prob.same = 0.2;

  if (note > 3 && note < 11 && note !=6 && note != 9) {  //notes outside this range could go out of bounds or is G1/C2
    prob.left1 = 0.025;
    prob.left2 = 0.2;
    prob.left3 = 0.025;
    prob.left4 = 0.15;
    prob.right1 = 0.025;
    prob.right2 = 0.2;
    prob.right3 = 0.025;
    prob.right4 = 0.15;
  } else if (note == 6) {                            //G1 is a special case in C Major
    prob.left1 = 0.025;
    prob.left2 = 0.2;
    prob.left3 = 0.025;
    prob.left4 = 0.15;
    prob.right1 = 0.025;
    prob.right2 = 0.1;
    prob.right3 = 0.25;
    prob.right4 = 0.025;
  } else if (note == 9) {                            //C2 is also special but in the reverse manner to G1
    prob.left1 = 0.025;
    prob.left2 = 0.1;
    prob.left3 = 0.25;
    prob.left4 = 0.025;
    prob.right1 = 0.025;
    prob.right2 = 0.2;
    prob.right3 = 0.025;
    prob.right4 = 0.15;
  }
  //I understand nested if statements would use less repeated code but it would also look disgusting

  switch(note) {
    case (A1):
      prob.left1 = prob.left2 = prob.left3 = prob.left4 = 0;
      prob.right1 = 0.05;
      prob.right2 = 0.4;
      prob.right3 = 0.05;
      prob.right4 = 0.3;
      break;
    case (B1):
      prob.left2 = prob.left3 = prob.left4 = 0;
      prob.right1 = 0.05;
      prob.right2 = 0.375;
      prob.right3 = 0.05;
      prob.right4 = 0.275;
      prob.left1 = 0.05;
      break;
    case (C1):
      prob.left3 = prob.left4 = 0;
      prob.right1 = 0.05;
      prob.right2 = 0.23;
      prob.right3 = 0.05;
      prob.right4 = 0.19;
      prob.left1 = 0.05;
      prob.left2 = 0.23;
      break;
    case (D1):
      prob.left4 = 0;
      prob.right1 = 0.05;
      prob.right2 = 0.21;
      prob.right3 = 0.05;
      prob.right4 = 0.16;
      prob.left1 = 0.05;
      prob.left2 = 0.21;
      prob.left3 = 0.05;
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
      prob.left1 = 0.05;
      prob.left2 = 0.21;
      prob.left3 = 0.05;
      prob.left4 = 0.16;
      prob.right1 = 0.05;
      prob.right2 = 0.21;
      prob.right3 = 0.05;
      break;
    case (F2):
      prob.right3 = prob.right4 = 0;
      prob.left1 = 0.05;
      prob.left2 = 0.23;
      prob.left3 = 0.05;
      prob.left4 = 0.19;
      prob.right1 = 0.05;
      prob.right2 =0.23;
      break;
    case (G2):
      prob.right2 = prob.right3 = prob.right4 = 0;
      prob.left1 = 0.05;
      prob.left2 = 0.375;
      prob.left3 = 0.05;
      prob.left4 = 0.275;
      prob.right1 = 0.05;
    case (A3):
      prob.right1 = prob.right2 = prob.right3 = prob.right4 = 0;
      prob.left1 = 0.05;
      prob.left2 = 0.4;
      prob.left3 = 0.05;
      prob.left4 = 0.3;
  }

}

//void shift(probability prob) {
//
//}

int main() {
  return 0;
}