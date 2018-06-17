#include "MINi.h"

/* MINinterpreter 2018
The MIN Interpreter takes MIN (minimal instrument notation) files and turns them into DAMN bytecode.
MINi interprets labels to produce the corresponding bytecode.
The first labels should be:

MIN
  INSTRS value
  TEMPO value
  FPB value
END

The next block should be instrument information, formatted as:

INSTRUMENT value
  OSCILLATOR 1
    ATK value
    DEC value
    SUS value
    REL value
    SHAPE value
    MIX value
    OCT value
    DETUNE value
  OSCILLATOR 2
    ATK value
    DEC value
    SUS value
    REL value
    SHAPE value
    MIX value
    OCT value
    DETUNE value
  OSCILLATOR 3
    ATK value
    DEC value
    SUS value
    REL value
    SHAPE value
    MIX value
    OCT value
    DETUNE value
END

The interpreter expects at most the number of instruments specified in the header.
If an instrument is missing it will fill out a default square wave instrument.

Note section begins with NOTES
The notes are laid out in frames:

pitch vel dur : pitch vel dur : ... : pitch vel dur

eg.

C4 28 0 : E4 28 0 : G4 28 0 : C5 28 0

*/

int get_note_val(char *spn)
{
  if (!strcmp(spn, "STOP")) return 0xFF;

  char _pitch[2];
  char _octave[80];
  char *ptr;
  int pitch;
  int octave;

  if (spn[1] == 'b' || spn[1] == '#') {
   strncpy(_pitch, spn, 2);
   strcpy(_octave, spn + 2);
  } else {
   strncpy(_pitch, spn, 1);
   _pitch[1] = '\0';
   strcpy(_octave, spn + 1);
  }

  octave = strtol(_octave, &ptr, 0);

  if (!strcmp(_pitch, "C")) {
   pitch = 0;
  } else if (!strcmp(_pitch, "C#") || !strcmp(_pitch, "Db")) {
   pitch = 1;
  } else if (!strcmp(_pitch, "D")) {
   pitch = 2;
  } else if (!strcmp(_pitch, "D#") || !strcmp(_pitch, "Eb")) {
   pitch = 3;
  } else if (!strcmp(_pitch, "E")) {
   pitch = 4;
  } else if (!strcmp(_pitch, "F")) {
   pitch = 5;
  } else if (!strcmp(_pitch, "F#") || !strcmp(_pitch, "Gb")) {
   pitch = 6;
  } else if (!strcmp(_pitch, "G")) {
   pitch = 7;
  } else if (!strcmp(_pitch, "G#") || !strcmp(_pitch, "Ab")) {
   pitch = 8;
  } else if (!strcmp(_pitch, "A")) {
   pitch = 9;
  } else if (!strcmp(_pitch, "A#") || !strcmp(_pitch, "Bb")) {
   pitch = 10;
  } else if (!strcmp(_pitch, "B")) {
   pitch = 11;
  } else return -1;

  return (12 * octave + 1) + pitch;
}

Frame *init_frame(int no_instr)
{
  Frame *frame = calloc(1, sizeof(Frame));
  frame->notes = calloc(no_instr, sizeof(Note));
  return frame;
}

void free_frame(Frame *frame)
{
  free(frame->notes);
}

void write_frame(Frame *frame, int no_instr, FILE *dest)
{
  for (int i = 0; i < no_instr; i++) {
    fwrite(&frame->notes[i].pitch, 1, 2, dest);
    fwrite(&frame->notes[i].sustain, 2, 1, dest);
  }
}

void end(FILE *dest)
{
  uint64_t max = -1;
  fwrite(&max, 4, 1, dest);
}

Instrument *init_instruments(int no_instr)
{
  Instrument *instruments = calloc(no_instr, sizeof(Instrument));
  for (int i = 0; i < no_instr; i++) {
    instruments[i].oscillators[1].attack = 20;
    instruments[i].oscillators[1].decay = 20;
    instruments[i].oscillators[1].sustain = 80;
    instruments[i].oscillators[1].release = 20;
    instruments[i].oscillators[1].wave = 0;
    instruments[i].oscillators[1].mix = 100;
    instruments[i].oscillators[1].octave = 0;
    instruments[i].oscillators[1].detune = 0;
  }
  return instruments;
}

void write_instruments(Instrument *instruments, int no_instr, FILE *dest)
{
  fseek(dest, 0x10, SEEK_SET);
  for (int i = 0; i < no_instr; i++) {
    for (int j = 0; j < 3; j++) {
      fwrite(&instruments[i].oscillators[j].attack, 2, 1, dest); // ADSR
      fwrite(&instruments[i].oscillators[j].decay, 2, 1, dest);
      fwrite(&instruments[i].oscillators[j].sustain, 2, 1, dest);
      fwrite(&instruments[i].oscillators[j].release, 2, 1, dest);
      fwrite(&instruments[i].oscillators[j].wave, 1, 1, dest);
      fwrite(&instruments[i].oscillators[j].mix, 1, 1, dest); // WAVE and MIX
      fwrite(&instruments[i].oscillators[j].octave, 2, 1, dest); // OCTAVE
      fwrite(&instruments[i].oscillators[j].detune, 4, 1, dest); // DETUNE
    }
  }
}

void write_header(Header *header, FILE *dest)
{
  uint32_t zero = 0;
  fseek(dest, 0, SEEK_SET);
  fwrite(header->damn, 1, 4, dest);
  fwrite(&header->instrument_count, 4, 1, dest);
  fwrite(&header->tempo, 1, 1, dest);
  fwrite(&header->fpb, 1, 1, dest);
  fwrite(&zero, 1, 4, dest);
  fwrite(&zero, 1, 2, dest);
}

int main(int argc, char const *argv[])
{

  printf("MINi 2018\n");

  if (argc != 3) {
    printf("Use arguments to input file params\n");
    return EXIT_FAILURE;
  }

  char *line_buffer = calloc(80, sizeof(char));
  assert(line_buffer != NULL);
  char *token;
  char *_value;
  long value;
  char *ptr;
  FILE *src = fopen(argv[1], "r");
  FILE *output = fopen(argv[2], "w");
  long line = 0; // Used for Advanced Debugging

  // Header preprocessing
  Header *header = malloc(sizeof(Header));
  assert(header != NULL);

  fgets(line_buffer, 80, src);
  line++;
  line_buffer = strtok(line_buffer, "\n");
  if (strcmp(line_buffer, "MIN")) {
    printf("Error at line %li: Unexpected token (Did you mean MIN?)\n", line);
    printf("--> %s\n", line_buffer);
    return EXIT_FAILURE;
  }

  strcpy(header->damn, "DAMN");
  int tempo_found, instr_found, fpb_found;
  tempo_found = instr_found = fpb_found = 0;

  do {
    fgets(line_buffer, 80, src);
    line++;
    token = strtok(line_buffer, " \n");
    _value = strtok(NULL, " \n");
    value = strtol(_value, &ptr, 0);
    if (!strcmp(token, "TEMPO")) {
      header->tempo = value & 0xFF;
      tempo_found = 1;
    } else if (!strcmp(token, "INSTRS")) {
      header->instrument_count = value;
      instr_found = 1;
    } else if (!strcmp(token, "FPB")) {
      header->fpb = value & 0xFF;
      fpb_found = 1;
    } else if (strcmp(token, "INSTRUMENT")) {
      printf("Error at line %li: Unexpected token \"%s\"\n", line, token);
      return EXIT_FAILURE;
    }
  } while (strcmp(token, "INSTRUMENT"));

  // At this point the first instrument token is already in memory.

  if (tempo_found && instr_found && fpb_found) {
    write_header(header, output);
  } else {
    printf("Missing tokens!!\n");
    if (!tempo_found) printf("--> TEMPO\n");
    if (!instr_found) printf("--> INSTRS\n");
    if (!fpb_found) printf("--> FPB\n");
    return EXIT_FAILURE;
  }

  Instrument *instrs = init_instruments(header->instrument_count);
  int cur_instr, cur_osc;
  cur_instr = cur_osc = 0;
  do {
    if (!strcmp(token, "INSTRUMENT")) {
      if (value < header->instrument_count && 0 <= value) {
        cur_instr = value;
      } else {
        printf("Error at line %li: Invalid value %li\n", line, value);
        return EXIT_FAILURE;
      }
    } else if (!strcmp(token, "OSCILLATOR")) {
      if (value - 1 < 3 && 0 <= value - 1) {
        cur_osc = value - 1;
      } else {
        printf("Error at line %li: Invalid value %li\n", line, value);
        return EXIT_FAILURE;
      }
    } else if (!strcmp(token, "ATK")) {
      if (value >= 0) {
        instrs[cur_instr].oscillators[cur_osc].attack = value;
      } else {
        printf("Error at line %li: Invalid value %li\n", line, value);
        return EXIT_FAILURE;
      }
    } else if (!strcmp(token, "DEC")) {
      if (value >= 0) {
        instrs[cur_instr].oscillators[cur_osc].decay = value;
      } else {
        printf("Error at line %li: Invalid value %li\n", line, value);
        return EXIT_FAILURE;
      }
    } else if (!strcmp(token, "SUS")) {
      if (value <= 100 && 0 <= value) {
        instrs[cur_instr].oscillators[cur_osc].sustain = value;
      } else {
        printf("Error at line %li: Invalid value %li\n", line, value);
        return EXIT_FAILURE;
      }
    } else if (!strcmp(token, "REL")) {
      if (value >= 0) {
        instrs[cur_instr].oscillators[cur_osc].release = value;
      } else {
        printf("Error at line %li: Invalid value %li\n", line, value);
        return EXIT_FAILURE;
      }
    } else if (!strcmp(token, "SHAPE")) {
      if (value >= 0 && value < 5) {
        instrs[cur_instr].oscillators[cur_osc].wave = value;
      } else {
        printf("Error at line %li: Invalid value %li\n", line, value);
        return EXIT_FAILURE;
      }
    } else if (!strcmp(token, "MIX")) {
      if (value >= 0 && value <= 100) {
        instrs[cur_instr].oscillators[cur_osc].mix = value;
      } else {
        printf("Error at line %li: Invalid value %li\n", line, value);
        return EXIT_FAILURE;
      }
    } else if (!strcmp(token, "OCTAVE")) {
      instrs[cur_instr].oscillators[cur_osc].octave = value;
    } else if (!strcmp(token, "DETUNE")) {
      instrs[cur_instr].oscillators[cur_osc].detune = value;
    } else if (strcmp(token, "NOTES")) {
      printf("Error at line %li: Unexpected token \"%s\"\n", line, token);
      return EXIT_FAILURE;
    }

    fgets(line_buffer, 80, src);
    line++;
    token = strtok(line_buffer, " \n");
    _value = strtok(NULL, " \n");
    value = strtol(_value, &ptr, 0);
  } while (strcmp(token, "NOTES"));

  write_instruments(instrs, header->instrument_count, output);

  //int end_init_line = line;
  Frame *current_frame = init_frame(header->instrument_count);
  char *note_block[header->instrument_count];
  char *note;
  char *vel;
  char *dur;

  while (fgets(line_buffer, 80, src) != NULL) {
    note_block[0] = strtok(line_buffer, ":\n");
    //printf("%s\n", note_block[0]);

    for (int i = 1; i < header->instrument_count; i++) {
      note_block[i] = strtok(NULL, ":\n");
      //printf("%s\n", note_block[i]);
    }

    for (int i = 0; i < header->instrument_count; i++) {
      note = strtok(note_block[i], " ");
      if (note != NULL) current_frame->notes[i].pitch = get_note_val(note);
      else current_frame->notes[i].pitch = 0;
      vel = strtok(NULL, " ");
      if (vel != NULL) current_frame->notes[i].velocity = strtol(vel, &ptr, 0);
      else current_frame->notes[i].velocity = 0;
      if (current_frame->notes[i].pitch == 0xFF) current_frame->notes[i].velocity = 0xFF;
      dur = strtok(NULL, " ");
      if (dur != NULL) current_frame->notes[i].sustain = strtol(dur, &ptr, 0);
      else current_frame->notes[i].sustain = 0;
//printf("NOTE = %u, VEL = %u, SUS = %u\n", current_frame->notes[i].pitch, current_frame->notes[i].velocity, current_frame->notes[i].sustain);
    }
    write_frame(current_frame, header->instrument_count, output);
    line++;
  }

  free_frame(current_frame);

  end(output);

  fclose(src);
  fclose(output);
  free(line_buffer);
  free(instrs);
  free(header);

  printf("MIN translated successfully!!\n");
  printf("Saranghaeyo!\n");
  return EXIT_SUCCESS;
}
