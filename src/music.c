#include "music.h"

//This is for C Major

typedef struct{
  double left1;
  double left2;
  double left3;
  double left4;
  double right1;
  double right2;
  double right3;
  double right4;
  double same;
}probability;

enum notes{A1, B1, C1, D1, E1, F1, G1, A2, B2, C2, D2, E2, F2, G2, A3};

void setProbability(notes note) {
  probability->same = 0.2;

  if (notes > 3 && notes < 11 && notes !=6) {          //notes outside this range could go out of bounds or is G1/C2
    probability->left1 = 0.025;
    probability->left2 = 0.2;
    probability->left3 = 0.025;
    probability->left4 = 0.15;
    probability->right1 = 0.025;
    probability->right2 = 0.2;
    probability->right3 = 0.025;
    probability->right4 = 0.15;
  } else if (notes == 6) {                            //G1 is a special case in C Major
    probability->left1 = 0.025;
    probability->left2 = 0.2;
    probability->left3 = 0.025;
    probability->left4 = 0.15;
    probability->right1 = 0.025;
    probability->right2 = 0.1;
    probability->right3 = 0.25;
    probability->right4 = 0.025;
  } else if (notes == 9) {                            //C2 is also special but in the reverse manner to G1
    probability->left1 = 0.025;
    probability->left2 = 0.1;
    probability->left3 = 0.25;
    probability->left4 = 0.025;
    probability->right1 = 0.025;
    probability->right2 = 0.2;
    probability->right3 = 0.025;
    probability->right4 = 0.15;
  }
  //I understand nested if statements would use less repeated code but it would also look disgusting

  switch(note) {
    case (A1):
      probability->left1 = probability->left2 = probability->left3 = probability->left4 = 0;
      probability->right1 = 0.05;
      probability->right2 = 0.4;
      probability->right3 = 0.05;
      probability->right4 = 0.3;
    case (B1):
      probability->left2 = probability->left3 = probability->left4 = 0;
      probability->right1 = 0.05;
      probability->right2 = 0.375;
      probability->right3 = 0.05;
      probability->right4 = 0.275;
      probability->left1 = 0.05;
    case (C1):
      probability->left3 = probability->left4 = 0;
      probability->right1 = 0.05;
      probability->right2 = 0.23;
      probability->right3 = 0.05;
      probability->right4 = 0.19;
      probability->left1 = 0.05;
      probability->left2 =0.23;
    case (D1):
      probability->left4 = 0;
      probability->right1 = 0.05;
      probability->right2 = 0.21;
      probability->right3 = 0.05;
      probability->right4 = 0.16;
      probability->left1 = 0.05;
      probability->left2 = 0.21;
      probability->left3 = 0.05;
    case (E2):
      probability->right4 = 0;
      probability->left1 = 0.05;
      probability->left2 = 0.21;
      probability->left3 = 0.05;
      probability->left4 = 0.16;
      probability->right1 = 0.05;
      probability->right2 = 0.21;
      probability->right3 = 0.05;
    case (F2):
      probability->right3 = probability->right4 = 0;
      probability->left1 = 0.05;
      probability->left2 = 0.23;
      probability->left3 = 0.05;
      probability->left4 = 0.19;
      probability->right1 = 0.05;
      probability->right2 =0.23;
    case (G2):
      probability->right2 = probability->right3 = probability->right4 = 0;
      probability->left1 = 0.05;
      probability->left2 = 0.375;
      probability->left3 = 0.05;
      probability->left4 = 0.275;
      probability->right1 = 0.05;
    case (A3):
      probability->right1 = probability->right2 = probability->right3 = probability->right4 = 0;
      probability->left1 = 0.05;
      probability->left2 = 0.4;
      probability->left3 = 0.05;
      probability->left4 = 0.3;
  }

}