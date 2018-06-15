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

int getRandomChordDegree(ChordProbability *chrp)
{
  // Assume the random generator has been initialized with a seed
  int random = rand() % 100;
  int ret = 0;
  if (random < chrp->I) ret = 0;
  else if (random < chrp->II) ret = 1;
  else if (random < chrp->III) ret = 2;
  else if (random < chrp->IV) ret = 3;
  else if (random < chrp->V) ret = 4;
  else if (random < chrp->VI) ret = 5;
  else if (random < chrp->VII) ret = 6;
  // Input some code here to change the probabilities maybe?
  return ret;
}

int getRandomNote(int tonic, int previous, JumpProbabilities *jp)
{
  while (previous < 48) previous += 12;
  while (previous > 83) previous -= 12;
  int degree = (previous - tonic) % 12;
  while (degree < 0) degree += 12;
  int random = rand() % 100;
  if (degree == 0) { // I
    if (random < jp->subV) return previous - 7; // P5
    else if (random < jp->subIV) return previous - 5; // P4
    else if (random < jp->subIII) return previous - 3; // m3
    else if (random < jp->subII) return previous - 1; // m2
    else if (random < jp->zero) return previous; // unison
    else if (random < jp->addII) return previous + 2; // M2
    else if (random < jp->addIII) return previous + 4; // M3
    else if (random < jp->addIV) return previous + 5; // P4
    else if (random < jp->addV) return previous + 7; // P5
  } else if (degree == 2) { // II
    if (random < jp->subV) return previous - 7; // P5
    else if (random < jp->subIV) return previous - 5; // P4
    else if (random < jp->subIII) return previous - 3; // m3
    else if (random < jp->subII) return previous - 2; // M2
    else if (random < jp->zero) return previous; // unison
    else if (random < jp->addII) return previous + 2; // M2
    else if (random < jp->addIII) return previous + 3; // m3
    else if (random < jp->addIV) return previous + 5; // P4
    else if (random < jp->addV) return previous + 7; // P5
  } else if (degree == 4) { // III
    if (random < jp->subV) return previous - 7; // P5
    else if (random < jp->subIV) return previous - 5; // P4
    else if (random < jp->subIII) return previous - 4; // M3
    else if (random < jp->subII) return previous - 2; // M2
    else if (random < jp->zero) return previous; // unison
    else if (random < jp->addII) return previous + 1; // m2
    else if (random < jp->addIII) return previous + 3; // m3
    else if (random < jp->addIV) return previous + 5; // P4
    else if (random < jp->addV) return previous + 7; // P5
  } else if (degree == 5) { // IV
    if (random < jp->subV) return previous - 6; // tritone
    else if (random < jp->subIV) return previous - 5; // P4
    else if (random < jp->subIII) return previous - 3; // m3
    else if (random < jp->subII) return previous - 1; // m2
    else if (random < jp->zero) return previous; // unison
    else if (random < jp->addII) return previous + 2; // M2
    else if (random < jp->addIII) return previous + 4; // M3
    else if (random < jp->addIV) return previous + 6; // tritone
    else if (random < jp->addV) return previous + 7; // P5
  } else if (degree == 7) { // V
    if (random < jp->subV) return previous - 7; // P5
    else if (random < jp->subIV) return previous - 5; // P4
    else if (random < jp->subIII) return previous - 3; // m3
    else if (random < jp->subII) return previous - 2; // M2
    else if (random < jp->zero) return previous; // unison
    else if (random < jp->addII) return previous + 2; // M2
    else if (random < jp->addIII) return previous + 4; // M3
    else if (random < jp->addIV) return previous + 5; // P4
    else if (random < jp->addV) return previous + 7; // P5
  } else if (degree == 9) { // VI
    if (random < jp->subV) return previous - 7; // P5
    else if (random < jp->subIV) return previous - 5; // P4
    else if (random < jp->subIII) return previous - 4; // M3
    else if (random < jp->subII) return previous - 2; // M2
    else if (random < jp->zero) return previous; // unison
    else if (random < jp->addII) return previous + 2; // M2
    else if (random < jp->addIII) return previous + 3; // m3
    else if (random < jp->addIV) return previous + 5; // P4
    else if (random < jp->addV) return previous + 7; // P5
  } else if (degree == 11) { // VII
    if (random < jp->subV) return previous - 7; // P5
    else if (random < jp->subIV) return previous - 6; // tritone
    else if (random < jp->subIII) return previous - 4; // M3
    else if (random < jp->subII) return previous - 2; // M2
    else if (random < jp->zero) return previous; // unison
    else if (random < jp->addII) return previous + 1; // m2
    else if (random < jp->addIII) return previous + 3; // m3
    else if (random < jp->addIV) return previous + 5; // P4
    else if (random < jp->addV) return previous + 6; // tritone
  }
  printf("Did not fall into bracket. Somethings wrong!!\n");
  return previous;
}

void writeChordToFile(int tonic, int degree, int inversion, FILE *project)
{
  // Assume the file has 3+ voices, and the pointer is in the right place

  int semitonal_offset;

  uint8_t velocity = 0x20;
  uint16_t sustain = 0;

  uint8_t root;
  uint8_t third;
  uint8_t fifth;

  switch (degree) {
    case 0: semitonal_offset = 0; break;
    case 1: semitonal_offset = 2; break;
    case 2: semitonal_offset = 4; break;
    case 3: semitonal_offset = 5; break;
    case 4: semitonal_offset = 7; break;
    case 5: semitonal_offset = 9; break;
    case 6: semitonal_offset = 11; break;
  }

  root = tonic + semitonal_offset;
  if (degree == 1 || degree == 2 || degree == 5 || degree == 7) third = root + 3;
  else third = root + 4;
  if (degree == 7) fifth = root + 6;
  else fifth = root + 7;

  if (inversion == 1) {
    root += 12;
  } else if (inversion == 2) {
    root += 12;
    third += 12;
  }

  fwrite(&root, 1, 1, project);
  fwrite(&velocity, 1, 1, project);
  fwrite(&sustain, 1, 2, project);

  fwrite(&third, 1, 1, project);
  fwrite(&velocity, 1, 1, project);
  fwrite(&sustain, 1, 2, project);

  fwrite(&fifth, 1, 1, project);
  fwrite(&velocity, 1, 1, project);
  fwrite(&sustain, 1, 2, project);

}

void writeZeroChord(FILE *project)
{
  uint64_t zero = 0;
  for (int i = 0; i < 3; i++) {
    fwrite(&zero, 4, 1, project);
  }
}

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

// Similar to above, but only accepts letter
Chromatics getNoteVal(char *note)
{
  if (!strcmp("A", note)) return A;
  if (!strcmp("A#", note) || !strcmp("Bb", note)) return As;
  if (!strcmp("B", note)) return B;
  if (!strcmp("C", note)) return C;
  if (!strcmp("C#", note) || !strcmp("Db", note)) return Cs;
  if (!strcmp("D", note)) return D;
  if (!strcmp("D#", note) || !strcmp("Eb", note)) return Ds;
  if (!strcmp("E", note)) return E;
  if (!strcmp("F", note)) return F;
  if (!strcmp("F#", note) || !strcmp("Gb", note)) return Fs;
  if (!strcmp("F", note)) return G;
  if (!strcmp("G#", note) || !strcmp("Ab", note)) return Gs;
  else return A;
}


int main(void) {
  char project[80];
  printf("What file to open? ");
  scanf("%s", project);

  srand(time(NULL));
  FILE *f = fopen(project, "wb");
  if (f == NULL)
  {
    printf("Error opening file!\n");
    exit(1);
  }

  ChordProbability chpr = {
    20, 30, 40, 60, 80, 90, 100
  };

  JumpProbabilities jp = {
    4, 13, 27, 46, 54, 73, 87, 96, 100
  };

  //Here is where we create the DAMN header so that the files can be read and understood
  char damn[] = {'D','A','M','N'};

  uint32_t *no_instr = malloc(sizeof(uint32_t));
  uint8_t *tempo = malloc(sizeof(uint8_t));
  uint8_t *fpb = malloc(sizeof(uint8_t));
  int *loop = malloc(sizeof(int));

  printf("Number of instruments: ");
  scanf("%u", no_instr);
  printf("Tempo: ");
  scanf("%hhu", tempo);
  printf("Frames per beat: ");
  scanf("%hhu", fpb);
  printf("Loop length: ");
  scanf("%d", loop);


  uint16_t non = 0x0;
  uint16_t attack = 0x5;
  uint16_t decay = 0x5;
  uint16_t sustain = 0x32;
  uint16_t release = 0x5;
  uint8_t waveshape = 0x0;
  uint8_t oscillator_volume = 0x64;
  uint16_t offset = 0x0;
  uint32_t detune = 0x0;
  uint64_t osc = 0x0;

  fwrite(damn, 1, 4, f);
  fwrite(no_instr, 1, 4, f);
  fwrite(tempo, 1, 1, f);
  fwrite(fpb, 1, 1, f);
  fwrite(&non, 1, 2, f);
  fwrite(&non, 1, 2, f);
  fwrite(&non, 1, 2, f);
  for (int j = 0; j < *no_instr; j++) {
    fwrite(&attack, 1, 2, f);
    fwrite(&decay, 1, 2, f);
    fwrite(&sustain, 1, 2, f);
    fwrite(&release, 1, 2, f);
    fwrite(&waveshape, 1, 1, f);
    fwrite(&oscillator_volume, 1, 1, f);
    fwrite(&offset, 1, 2, f);
    fwrite(&detune, 1, 4, f);
    for (int i = 0; i < 8; i++) fwrite(&osc, 1, 4, f);
  }

  srand(time(NULL));
  char *note = malloc(sizeof(char*));
  if (note == NULL) {
    printf("Note allocation in memory failed\n");
    exit(EXIT_FAILURE);
  }

  printf("Starting note: ");
  scanf("%s", note);
  //uint8_t *muse;
  //uint8_t temp;
  uint8_t tonic = getNoteVal(note);
  uint8_t melodies[*no_instr - 3];
  for (int i = 3; i < *no_instr; i++) melodies[i] = tonic;
  uint8_t velocity = 0x100 / *no_instr;
  int creation_percentage = *loop / 100;
  for (int i = 0; i < *loop; ++i) {
    if ((i % *fpb) == 0) writeChordToFile(tonic - 24, getRandomChordDegree(&chpr), rand() % 3, f);
    else writeZeroChord(f);

    /* Sam's code
    muse = &temp;
    fwrite(muse, 1, 1, f);
    fwrite(&velocity, 1, 1, f);
    fwrite(&non, 1, 2, f);
    setProbability(temp);
    temp = shift(temp) % 74 + 23;
    */

    // My more musical code
    for (int i = 3; i < *no_instr; i++) {
      melodies[i] = getRandomNote(tonic, melodies[i], &jp);
      fwrite(melodies + i, 1, 1, f);
      fwrite(&velocity, 1, 1, f);
      fwrite(&non, 1, 2, f);
    }
    if (i % creation_percentage == 0) printf("\rGeneration percentage: %u%%", (i * 100) / *loop);
  }
  printf("\rGeneration percentage: 100%%\n");

  uint32_t end = 0xFFFFFFFF;
  fwrite(&end, 4, 1, f);
  free(note);
  free(no_instr);
  free(tempo);
  free(fpb);
  free(loop);
  fclose(f);

  printf("\nPlease render %s with the DAMN Tracker System!!\n", project);
  printf("Enjoy your random music!!\n");
  return 0;
}
