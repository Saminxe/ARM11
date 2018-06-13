#include "damnwriter.h"

void writeHeader(FILE* project, int instrCount, int tempo, int fpb)
{
  Header header = {"DAMN", instrCount, tempo, fpb};
  fwrite(&header, 1, 10, project);
  uint64_t zero = 0;
  fwrite(&zero, 1, 6, project);
}

void initDefaultInstruments(FILE *project, int instrument_count)
{
  fseek(project, 0x10, SEEK_SET);
  OscillatorParameters osc_default = {
    10, 10, 75, 10,
    0, 100, 0, 0
  };
  OscillatorParameters zero = {0,0,0,0,0,0,0,0};
  InstrumentParameters init = {osc_default, zero, zero};
  for (int i = 0; i < instrument_count; i++) {
    fwrite(&init, 1, 0x30, project);
  }
}

void writeInstrument(FILE *project, InstrumentParameters instrument)
{
  fwrite(&instrument, 1, 0x30, project);
}

void printRainbow(char *string)
{
  int len = strlen(string);
  if (has_colors()) {
    start_color();
    init_pair(1, COLOR_RED, COLOR_WHITE);
    init_pair(2, COLOR_GREEN, COLOR_WHITE);
    init_pair(3, COLOR_YELLOW, COLOR_WHITE);
    init_pair(4, COLOR_BLUE, COLOR_WHITE);
    init_pair(5, COLOR_MAGENTA, COLOR_WHITE);
    init_pair(6, COLOR_CYAN, COLOR_WHITE);
    char ch[2];
    ch[1] = '\0';
    for (int i = 0; i < len; i++) {
      ch[0] = string[i];
      attron(COLOR_PAIR((i % 6) + 1));
      printw(ch);
    }
  }
  else printw(string);
}

int main()
{
  int ch;
  initscr();			/* Start curses mode 		*/
	raw();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
  attron(A_BOLD);
  printw("DAMN Tracker Writer");
  attroff(A_BOLD);
  printw(" © 2018 by memes\n");
  printw("File to open: ");
  char buffer[80];
  getstr(buffer); // Gets the filename
  FILE *project = fopen(buffer, "r+");
  if (project == NULL) {
    printw("Created new project %s\n", buffer);
    project = fopen(buffer, "w+");
    printw("Project instrument count? ");
    getstr(buffer);
    char *ptr;
    long instrCount = strtol(buffer, &ptr, 0);
    printw("Project tempo? ");
    getstr(buffer);
    long tempo = strtol(buffer, &ptr, 0);
    printw("Project frames per beat? ");
    getstr(buffer);
    long fpb = strtol(buffer, &ptr, 0);
    writeHeader(project, instrCount, tempo, fpb);
    initDefaultInstruments(project, instrCount);
  } else {
    printw("Opened project %s", buffer);
  }

  fseek(project, 0, SEEK_SET);
  fread(buffer, 1, 4, project);
  buffer[4] = '\0';
  if (!strcmp("DAMN", buffer))

  getch();			/* Wait for user input */
	endwin();			/* End curses mode		  */
  fclose(project);
	return 0;
}
