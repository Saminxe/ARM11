#include "tracker.h"

/*** Command line version of tracker ***/

/***
INTRODUCING: The DAMN tracker!!!
DAMN: Digitally Approximated Music using NO-L

Reads binary .damn format and synthesizes it using the NO-L 3xOsc Synth!!!

INPUT:
A .damn file!!!

OUTPUT:
Since this is 'approximated' music, it's only mono and will write to a wave file.

DESCRIPTION of .DAMN FORMAT:
  HEADER:
    0x00 - 0x03: 4 bytes saying "DAMN" in big-endian (rest is little-endian)
    0x04 - 0x07: 4 bytes for the number of instruments/tracking channels (max 16, might change)
    0x08 - 0x0F: Metadata:
      0x08: Tempo
      0x09: Frames per beat (normally 4; demisemiquaver is minimum quantization)
      0x0A: Other stuff that might be useful later, like a gun that shoots a smaller gun that shoots a knife which explodes on impact
    0x10 onwards: (no. of instruments * 48) bytes for ADSR and waveshape information of the instruments:
      OFFSET:
        OSC1 (0x00)
          0x00 - 0x01 : Attack length in ms
          0x02 - 0x03 : Decay length in ms
          0x04 - 0x05 : Sustain level in %
          0x06 - 0x07 : Release length in ms
          0x08        : Wave shape
          0x09        : Mix in %
          0x0A - 0x0B : Octave offset
          0x0C - 0x0F : Detune in cents
        OSC2 (0x10)
        OSC3 (0x20)

  DATA:
    Each frame is (no. of instruments * 4) bytes large:
    FRAME SECTION:
      0x0: Note - 0 = C0, 131 = B9, 255 = STOP
      0x1: Dynamic mix of the note, 0 = nonexistent, 255 = max overdrive!!!
      0x2 - 0x3: Sustain/Pulse duration, 0 = sustain, any other number represents pulse duration in frames.
        The sustain note stops when a STOP note appears on the channel again.
    After the expected number of instrument notes is read, the next 4 bytes is interpreted as a note in a new frame.
    The special note 0x00000000 means no new notes are to be played.
    If a sustain note is played, the tracker will count the number of frames until the next note, and then use this as the duration.
    The reading continues until the file ends OR a -1 frame is found (ie 0xFFFFFFFF)
***/

/***
MODULE: THE NO-L 3xOSC SYNTHESIZER
NO-L for (definitely) Not Logical Synthesizer (pronounced Noelle (pronounced no l))
3xOsc for 3 oscillators!

BEAUTIFUL FEATURES OF NO-L:
 - Produce square, saw, triangle and sine waves, plus noise!!!
 - Support for dynamic enveloping!!!
 - Instruments support multiple oscillators at different pitches!!!
 - Writes directly to PCM!!!
 - Change the tuning so u can be electro-baroque af!!!
 - Supports any sample rate (don't make it less than 40000Hz!!!)!!!
 - Built in constant representing the twelfth root of two!!!
***/

int max(int a, int b)
{
  return (a < b) ? b : a;
}

int min(int a, int b)
{
  return (a < b) ? a : b;
}

void printInstruments(InstrParams *instruments, int instrument_count)
{
  for (int i = 0; i < instrument_count; i++) {

    uint16_t atk1 = instruments[i].osc1.envelope.atk;
    uint16_t dec1 = instruments[i].osc1.envelope.dec;
    uint16_t sus1 = instruments[i].osc1.envelope.sus;
    uint16_t rel1 = instruments[i].osc1.envelope.rel;

    uint8_t shape1 = instruments[i].osc1.waveshape;
    uint8_t mix1 = instruments[i].osc1.mix;
    int16_t octave1 = instruments[i].osc1.octave;
    int32_t detune1 = instruments[i].osc1.detune;

    uint16_t atk2 = instruments[i].osc2.envelope.atk;
    uint16_t dec2 = instruments[i].osc2.envelope.dec;
    uint16_t sus2 = instruments[i].osc2.envelope.sus;
    uint16_t rel2 = instruments[i].osc2.envelope.rel;

    uint8_t shape2 = instruments[i].osc2.waveshape;
    uint8_t mix2 = instruments[i].osc2.mix;
    int16_t octave2 = instruments[i].osc2.octave;
    int32_t detune2 = instruments[i].osc2.detune;

    uint16_t atk3 = instruments[i].osc3.envelope.atk;
    uint16_t dec3 = instruments[i].osc3.envelope.dec;
    uint16_t sus3 = instruments[i].osc3.envelope.sus;
    uint16_t rel3 = instruments[i].osc3.envelope.rel;

    uint8_t shape3 = instruments[i].osc3.waveshape;
    uint8_t mix3 = instruments[i].osc3.mix;
    int16_t octave3 = instruments[i].osc3.octave;
    int32_t detune3 = instruments[i].osc3.detune;

    printf("Instrument %u OSC 1:\n", i);
    printf("ATK = %u, DEC = %u, SUS = %u%%, REL = %u\n", atk1, dec1, sus1, rel1);
    printf("Shape = %u, Mix Level = %u%%, Octave offset = %d, Detune = %d\n", shape1, mix1, octave1, detune1);
    printf("Instrument %u OSC 2:\n", i);
    printf("ATK = %u, DEC = %u, SUS = %u%%, REL = %u\n", atk2, dec2, sus2, rel2);
    printf("Shape = %u, Mix Level = %u%%, Octave offset = %d, Detune = %d\n", shape2, mix2, octave2, detune2);
    printf("Instrument %u OSC 3:\n", i);
    printf("ATK = %u, DEC = %u, SUS = %u%%, REL = %u\n", atk3, dec3, sus3, rel3);
    printf("Shape = %u, Mix Level = %u%%, Octave offset = %d, Detune = %d\n", shape3, mix3, octave3, detune3);
    printf("\n");
  }
}

/***
Envelope: Denotes the volume envelople
Waveshape: Denotes the type of wave to produce
Frequency: The frequency of the note in Hz
Duration: The length of the note in ms
This is a 16-bit depth oscillator.
***/

int16_t *osc(ADSR envelope, Shape wave, double freq, int duration)
{
  // Calculates the length of envelope sections in frames
  int atk_len = (envelope.atk * SAMPLE_RATE) / 1000;
  int dec_len = (envelope.dec * SAMPLE_RATE) / 1000;
  int rel_len = (envelope.rel * SAMPLE_RATE) / 1000;

  // Initializes the waveform to be returned.
  uint32_t wave_length = ((duration * SAMPLE_RATE) / 1000) + rel_len;
  int16_t *waveform = calloc(wave_length, sizeof(int16_t));
  assert(waveform != NULL);
  uint32_t wave_ptr = 0;
  int16_t sus_amplitude = (int16_t) (((double) envelope.sus / 100) * INT16_MAX);

  if (sus_amplitude != 0) {

    int sus_len = wave_length - (atk_len + dec_len + rel_len);

    // Populates the waveform with the volume envelople
    // Attack profile
    if (atk_len != 0) {
      for (int i = 0; i < atk_len; i++) {
        if (wave_ptr < wave_length) {
          *(waveform + wave_ptr) = (int16_t) (((double) i / atk_len) * INT16_MAX);
          wave_ptr++;
        }
      }
    }

    // Decay profile
    if (dec_len != 0) {
      for (int i = 0; i < dec_len; i++) {
        if (wave_ptr < wave_length) {
          *(waveform + wave_ptr) = INT16_MAX - (int16_t) (((double) i / dec_len) * (INT16_MAX - sus_amplitude));
          wave_ptr++;
        }
      }
    }

    // Sustain profile
    for (int i = 0; i < sus_len; i++) {
      if (wave_ptr < wave_length) {
        *(waveform + wave_ptr) = sus_amplitude;
        wave_ptr++;
      }
    }

    // Release profile
    if (rel_len != 0) {
      for (int i = 0; i < rel_len; i++) {
        if (wave_ptr < wave_length) {
          *(waveform + wave_ptr) = sus_amplitude - (int16_t) (((double) i / rel_len) * sus_amplitude);
          wave_ptr++;
        }
      }
    }

    wave_ptr = 0; // Resets the pointer

    // Calculates the number of frames a single wave takes up.
    int period = (int) ((1.0 / freq) * SAMPLE_RATE);

    // Populates the waveform with the appropriate wave shape.
    // For square waves, the pulse width ratio is assumed to be 50%.
    if (wave == SQUARE) {
      while (wave_ptr < wave_length) {
        int16_t envelope = *(waveform + wave_ptr);
        if ((wave_ptr % period) < (period / 2)) { // we can change the PW here
          *(waveform + wave_ptr) = envelope;
        } else {
          *(waveform + wave_ptr) = - envelope;
        }
        //printf("Frame %u: %d\n", wave_ptr, *(waveform + wave_ptr));
        wave_ptr++;
      }
    } else if (wave == SAW) {
      while (wave_ptr < wave_length) {
        double envelope_ratio = (double) *(waveform + wave_ptr) / INT16_MAX;
        int timer = wave_ptr % period;
        int16_t value = ((double) (timer * 2) / period) * INT16_MAX;
        if (value >= INT16_MAX) {
          value = value - (2 * INT16_MAX);
        }
        value = (int16_t) (envelope_ratio * value);
        *(waveform + wave_ptr) = value;
        //printf("Frame %u: %d\n", wave_ptr, value);
        wave_ptr++;
      }
    } else if (wave == TRIANGLE) {
      while (wave_ptr < wave_length) {
        double envelope_ratio = (double) *(waveform + wave_ptr) / INT16_MAX;
        int timer = wave_ptr % period;
        int16_t value = ((double) (timer * 4) / period) * INT16_MAX;
        if (value >= INT16_MAX && value < 3 * INT16_MAX) {
          value = 2 * INT16_MAX - value;
        } else if (value >= 3 * INT16_MAX) {
          value = value - 4 * INT16_MAX;
        }
        value = (int16_t) (envelope_ratio * value);
        *(waveform + wave_ptr) = value;
        //printf("Frame %u: %d\n", wave_ptr, value);
        wave_ptr++;
      }
    } else if (wave == NOISE) { // v bad implementation of noise, do not use
      srand(time(NULL));
      while (wave_ptr < wave_length) {
        double envelope_ratio = (double) *(waveform + wave_ptr) / INT16_MAX;
        int16_t value = (rand() % UINT16_MAX) - INT16_MAX;
        value = (int16_t) (envelope_ratio * value);
        *(waveform + wave_ptr) = value;
        //printf("Frame %u: %d\n", wave_ptr, value);
        wave_ptr++;
      }
    } else if (wave == SINE) {
      while (wave_ptr < wave_length) {
        double envelope_ratio = (double) *(waveform + wave_ptr) / INT16_MAX;
        int16_t value = (int) (sin(((2 * M_PI) / period) * wave_ptr) * INT16_MAX);
        value = (int16_t) (envelope_ratio * value);
        *(waveform + wave_ptr) = value;
        //printf("Frame %u: %d\n", wave_ptr, value);
        wave_ptr++;
      }
    }
  }
  return waveform;
}

int16_t *scale(double scalar, int16_t *wave, int wave_size)
{
  for (int i = 0; i < wave_size; i++) {
    wave[i] = (int16_t) ((double) wave[i] * scalar);
  }
  return wave;
}

void superpose(int16_t *wave1, int size1, int16_t *wave2, int size2)
{
  for (int i = 0; i < size1; i++) if (i < size2) wave1[i] += wave2[i];
}

/***
Instruments consists of 3 oscillators.

They can be set to play at octave offsets from the main note, or be detuned
up to 1200 cents in each direction (+/-).

Each the mix of each oscillator can be controlled; if the mix is 0, it will not
be played.

Each oscillator's output is added to each other.

Post filters can then control the sound of the instrument.
***/

int16_t *instrument(InstrParams parameters, int note, int duration, uint8_t velocity)
{
  int max_release = max(max(
    parameters.osc1.envelope.rel,
    parameters.osc2.envelope.rel),
    parameters.osc3.envelope.rel
  );
  int wave_length = ((duration + max_release) * SAMPLE_RATE) / 1000;
  int16_t *waveform = calloc(wave_length, sizeof(int16_t));
  int osc1_size = ((parameters.osc1.envelope.rel + duration) * SAMPLE_RATE) / 1000;
  int osc2_size = ((parameters.osc2.envelope.rel + duration) * SAMPLE_RATE) / 1000;
  int osc3_size = ((parameters.osc3.envelope.rel + duration) * SAMPLE_RATE) / 1000;
  double osc1_note =
    note + (parameters.osc1.detune / 100) + (12 * parameters.osc1.octave);
  double osc1_note_offset = osc1_note - 69;
  double osc1_frequency = (A) * (
    (osc1_note_offset > 0) ? pow(FREQUENCY_RATIO, osc1_note_offset) :
    1 / pow(FREQUENCY_RATIO, -osc1_note_offset));
  double osc2_note =
    note + (parameters.osc2.detune / 100) + (12 * parameters.osc2.octave);
  double osc2_note_offset = osc2_note - 69;
  double osc2_frequency = (A) * (
    (osc2_note_offset > 0) ? pow(FREQUENCY_RATIO, osc2_note_offset) :
    1 / pow(FREQUENCY_RATIO, -osc2_note_offset));
  double osc3_note =
    note + (parameters.osc3.detune / 100) + (12 * parameters.osc3.octave);
  double osc3_note_offset = osc3_note - 69;
  double osc3_frequency = (A) * (
    (osc3_note_offset > 0) ? pow(FREQUENCY_RATIO, osc3_note_offset) :
    1 / pow(FREQUENCY_RATIO, -osc3_note_offset));
  int16_t *osc1_wave = osc(parameters.osc1.envelope, parameters.osc1.waveshape, osc1_frequency, duration);
  int16_t *osc2_wave = osc(parameters.osc2.envelope, parameters.osc2.waveshape, osc2_frequency, duration);
  int16_t *osc3_wave = osc(parameters.osc3.envelope, parameters.osc3.waveshape, osc3_frequency, duration);

  double osc1_mix = (double) parameters.osc1.mix / 100;
  double osc2_mix = (double) parameters.osc2.mix / 100;
  double osc3_mix = (double) parameters.osc3.mix / 100;

  osc1_wave = scale(osc1_mix, osc1_wave, osc1_size);
  osc2_wave = scale(osc2_mix, osc2_wave, osc2_size);
  osc3_wave = scale(osc3_mix, osc3_wave, osc3_size);

  superpose(waveform, wave_length, osc1_wave, osc1_size);
  superpose(waveform, wave_length, osc2_wave, osc2_size);
  superpose(waveform, wave_length, osc3_wave, osc3_size);

  free(osc1_wave);
  free(osc2_wave);
  free(osc3_wave);

  double velocity_scalar = (double) velocity / 255;

  waveform = scale(velocity_scalar, waveform, wave_length);

  //printf("Played %u for %ums\n", note, duration);
  return waveform;
}

void printPitch(int pitch)
{
  // Prints out the note represented in scientific pitch notation
  switch (pitch % 12) {
    case 0: printf("C"); break;
    case 1: printf("C#/Db"); break;
    case 2: printf("D"); break;
    case 3: printf("D#/Eb"); break;
    case 4: printf("E"); break;
    case 5: printf("F"); break;
    case 6: printf("F#/Gb"); break;
    case 7: printf("G"); break;
    case 8: printf("G#/Ab"); break;
    case 9: printf("A"); break;
    case 10: printf("A#/Bb"); break;
    case 11: printf("B"); break;
  }
  printf("%u\n", (pitch / 12) - 1);
}

int16_t *mergeWaveformAtOffset(int16_t *master, long long master_length, int16_t *wave, long long wave_length, long long offset)
{
  long long sample_offset = (offset * SAMPLE_RATE) / 1000;
  for (int i = 0; i < wave_length; i++) {
    if (sample_offset + i >= master_length) break;
    int val = *(master + sample_offset + i) + *(wave + i);
    *(master + sample_offset + i) = (val > INT16_MAX) ? INT16_MAX : val;
  }
  return master;
}

/*** Wavewriter code ***/
int isBigEndian() {
  int test = 1;
  char *p = (char*)&test;
  return p[0] == 0;
}

void reverseEndianness(const long long int size, void* value){
  int i;
  char result[32];
  for (i=0; i<size; i+=1) {
    result[i] = ((char*)value)[size-i-1];
  }
  for (i=0; i<size; i+=1) {
    ((char*)value)[i] = result[i];
  }
}

void toBigEndian(const long long int size, void* value){
  char needsFix = !((1 && isBigEndian()) || (0 && !isBigEndian()));
  if (needsFix) {
    reverseEndianness(size,value);
  }
}

void toLittleEndian(const long long int size, void* value){
  char needsFix = !((0 && isBigEndian()) || (1 && !isBigEndian()));
  if (needsFix) {
    reverseEndianness(size,value);
  }
}

WaveHeader makeWaveHeader(int const sampleRate, short int const numChannels, short int const bitsPerSample)
{
  WaveHeader myHeader;
  myHeader.chunkId[0] = 'R';
  myHeader.chunkId[1] = 'I';
  myHeader.chunkId[2] = 'F';
  myHeader.chunkId[3] = 'F';
  myHeader.format[0] = 'W';
  myHeader.format[1] = 'A';
  myHeader.format[2] = 'V';
  myHeader.format[3] = 'E';
  myHeader.subChunk1Id[0] = 'f';
  myHeader.subChunk1Id[1] = 'm';
  myHeader.subChunk1Id[2] = 't';
  myHeader.subChunk1Id[3] = ' ';
  myHeader.audioFormat = 1;
  myHeader.numChannels = numChannels;
  myHeader.sampleRate = sampleRate;
  myHeader.bitsPerSample = bitsPerSample;
  myHeader.byteRate = myHeader.sampleRate * myHeader.numChannels * myHeader.bitsPerSample / 8;
  myHeader.blockAlign = myHeader.numChannels * myHeader.bitsPerSample/8;
  myHeader.subChunk2Id[0] = 'd';
  myHeader.subChunk2Id[1] = 'a';
  myHeader.subChunk2Id[2] = 't';
  myHeader.subChunk2Id[3] = 'a';
  myHeader.chunkSize = 4+8+16+8+0;
  myHeader.subChunk1Size = 16;
  myHeader.subChunk2Size = 0;
  return myHeader;
}

Wave makeWave(int const sampleRate, short int const numChannels, short int const bitsPerSample)
{
  Wave myWave;
  myWave.header = makeWaveHeader(sampleRate,numChannels,bitsPerSample);
  return myWave;
}

void freeWave(Wave* wave)
{
  free(wave->data);
}

void waveSetDuration(Wave* wave, const long long int milliseconds)
{
  long long int totalBytes = (long long int) ((wave->header.byteRate * milliseconds) / 1000);
  wave->data = (char*)malloc(totalBytes);
  wave->index = 0;
  wave->size = totalBytes;
  wave->nSamples = (long long int) wave->header.numChannels * ((wave->header.sampleRate * milliseconds) / 1000);
  wave->header.chunkSize = 4+8+16+8+totalBytes;
  wave->header.subChunk2Size = totalBytes;
}

void waveAddSample16(Wave* wave, const int16_t sample16bit)
{
  char *sample;
  toLittleEndian(2, (void*) &sample16bit);
  sample = (char*)&sample16bit;
  wave->data[wave->index + 0] = sample[0];
  wave->data[wave->index + 1] = sample[1];
  wave->index += 2;
}

void waveToFile(Wave* wave, const char* filename)
{
  toLittleEndian(sizeof(int), (void*)&(wave->header.chunkSize));
  toLittleEndian(sizeof(int), (void*)&(wave->header.subChunk1Size));
  toLittleEndian(sizeof(short int), (void*)&(wave->header.audioFormat));
  toLittleEndian(sizeof(short int), (void*)&(wave->header.numChannels));
  toLittleEndian(sizeof(int), (void*)&(wave->header.sampleRate));
  toLittleEndian(sizeof(int), (void*)&(wave->header.byteRate));
  toLittleEndian(sizeof(short int), (void*)&(wave->header.blockAlign));
  toLittleEndian(sizeof(short int), (void*)&(wave->header.bitsPerSample));
  toLittleEndian(sizeof(int), (void*)&(wave->header.subChunk2Size));
  FILE *file;
  file = fopen(filename, "wb");
  fwrite( &(wave->header), sizeof(WaveHeader), 1, file );
  fwrite( (void*)(wave->data), sizeof(char), wave->size, file );
  fclose( file );
  toLittleEndian(sizeof(int), (void*)&(wave->header.chunkSize));
  toLittleEndian(sizeof(int), (void*)&(wave->header.subChunk1Size));
  toLittleEndian(sizeof(short int), (void*)&(wave->header.audioFormat));
  toLittleEndian(sizeof(short int), (void*)&(wave->header.numChannels));
  toLittleEndian(sizeof(int), (void*)&(wave->header.sampleRate));
  toLittleEndian(sizeof(int), (void*)&(wave->header.byteRate));
  toLittleEndian(sizeof(short int), (void*)&(wave->header.blockAlign));
  toLittleEndian(sizeof(short int), (void*)&(wave->header.bitsPerSample));
  toLittleEndian(sizeof(int), (void*)&(wave->header.subChunk2Size));
}

int main(int argc, char *argv[]) {
  printf("DAMN TRACKER made with blood, sweat and tears\n");
  printf("Powered by NO-L synthesizer\n");

  if (argc != 3) {
    printf("Usage: %s, input, output\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE *inp = fopen(argv[1], "rb");

  if (inp == NULL) {
    printf("File load failure! ");
    return EXIT_FAILURE;
  }

  fseek(inp, 0, SEEK_END);
  long fsize = ftell(inp); // Get the size of the file
  fseek(inp, 0, SEEK_SET); // Reset the seeker

  char damn[5];
  fread(damn, 1, 4, inp);
  damn[4] = '\0';
  if (strcmp(damn, "DAMN")) {
    printf("%s", damn);
    printf("File is not DAMN format! ");
    return EXIT_FAILURE;
  }

  uint32_t instrument_count;
  fread(&instrument_count, 4, 1, inp);
  if (instrument_count == 0) {
    printf("Invalid amount of instruments!");
    return EXIT_FAILURE;
  }

  uint8_t tempo;
  fread(&tempo, 1, 1, inp);
  if (tempo == 0) {
    printf("Invalid tempo!");
    return EXIT_FAILURE;
  }

  uint8_t fpb;
  fread(&fpb, 1, 1, inp);
  if (fpb == 0) {
    printf("Invalid frames per bar!");
    return EXIT_FAILURE;
  }

  printf("\nPlease review project settings: \n");

  printf("Number of voices: %u\n", instrument_count);
  fseek(inp, 6, SEEK_CUR);
  printf("Tempo = %u, Frames/beat = %u\n", tempo, fpb);
  int frame_duration = (int) (((double) 1000.0 / ((double) tempo / 60)) / fpb);
  unsigned long long duration = ((fsize - (0x10 + 0x30 * instrument_count)) / (4 * instrument_count)) * frame_duration + 100; // pad by 100ms
  unsigned long long num_samples = (duration * SAMPLE_RATE) / 1000;
  printf("Total duration = %llums, %llu samples\n", duration, num_samples);
  printf("\n");

  InstrParams instruments[instrument_count];
  for (int i = 0; i < instrument_count; i++) {
    Osc oscillators[3];
    long max_release = 0;
    for (int j = 0; j < 3; j++) {
      uint16_t atk;
      uint16_t dec;
      uint16_t sus;
      uint16_t rel;

      uint8_t shape;
      uint8_t mix;
      int16_t octave;
      int32_t detune;

      fread(&atk, 2, 1, inp);
      fread(&dec, 2, 1, inp);
      fread(&sus, 2, 1, inp);
      fread(&rel, 2, 1, inp);

      fread(&shape, 1, 1, inp);
      fread(&mix, 1, 1, inp);
      fread(&octave, 2, 1, inp);
      fread(&detune, 4, 1, inp);

      ADSR osc_env = {atk, dec, sus, rel};
      Osc oscillator = {osc_env, shape, mix, octave, detune};
      oscillators[j] = oscillator;
      if (max_release < rel) max_release = rel;
    }
    InstrParams instrument = {oscillators[0], oscillators[1], oscillators[2], max_release};
    instruments[i] = instrument;
  }

  long sustain_ctrs[instrument_count];
  for (int i = 0; i < instrument_count; i++) sustain_ctrs[i] = 0; // Sustain counters
  long sustain_ptrs[instrument_count];
  for (int i = 0; i < instrument_count; i++) sustain_ptrs[i] = 0; // Sustain pointers
  long sustain_vals[instrument_count];
  for (int i = 0; i < instrument_count; i++) sustain_vals[i] = 0; // Sustained note values
  long sustain_vels[instrument_count];
  for (int i = 0; i < instrument_count; i++) sustain_vels[i] = 0; // Sustained note velocities
  uint8_t buffer[instrument_count][4];
  unsigned long long frame = 0;
  int16_t *master = calloc(num_samples, sizeof(int16_t)); // Initialize the master track
  unsigned long long frame_max = (fsize - (0x10 + 0x30 * instrument_count)) / (4 * instrument_count);
  //int synthesis_percent = frame_max / 100;

  // Parsing Loop
  while (frame < frame_max) {
    for (int i = 0; i < instrument_count; i++) {
      fread(buffer[i], 1, 4, inp);
      if (buffer[i][1] != 0) {
        if (sustain_ctrs[i] != 0) {
          int play_dur = sustain_ctrs[i] * frame_duration;
          int offset = sustain_ptrs[i] * frame_duration;
          int16_t *play_note = instrument(instruments[i], sustain_vals[i], play_dur, sustain_vels[i]);
          mergeWaveformAtOffset(master, num_samples, play_note, ((play_dur+ instruments[i].max_release) * SAMPLE_RATE) / 1000, offset);
          free(play_note);
          sustain_ctrs[i] = 0;
        }
        uint16_t note_dur = (buffer[i][3] << 8) | buffer[i][2];
          if (buffer[i][0] != 0xFF) {
          if (note_dur == 0) {
            sustain_ctrs[i] = 1;
            sustain_ptrs[i] = frame;
            sustain_vals[i] = buffer[i][0];
            sustain_vels[i] = buffer[i][1];
          } else {
            int16_t *play_note = instrument(instruments[i], buffer[i][0], note_dur, buffer[i][1]);
            mergeWaveformAtOffset(master, num_samples, play_note, ((note_dur + instruments[i].max_release) * SAMPLE_RATE) / 1000, frame * frame_duration);
            free(play_note);
          }
        }
      } else {
        if (sustain_ctrs[i] != 0) sustain_ctrs[i]++;
      }
      if (buffer[i][0] == 0xFF && buffer[i][1] == 0xFF && buffer[i][2] == 0xFF && buffer[i][3] == 0xFF) {
        printf("END SIGNAL\n");
        goto END;
      }
    }
    frame++;
  }
  END: printf("\rSynthesis progress: 100%%\n");
  Wave master_wave = makeWave(SAMPLE_RATE, 1, 16);
  waveSetDuration(&master_wave, duration);
  //int write_percent = num_samples / 100;
  for (long long master_ptr = 0; master_ptr < num_samples; master_ptr++) {
    waveAddSample16(&master_wave, master[master_ptr]);
  }
  waveToFile(&master_wave, argv[2]);
  freeWave(&master_wave);
  printf("\rWriting progress: 100%%\n");
  fflush(stdout);

  fclose(inp);
  free(master);

  return EXIT_SUCCESS;
}
