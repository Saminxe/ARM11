// Code taken from http://blog.acipo.com/generating-wave-files-in-c/
// Modified for our purpose

#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <stdint.h>

typedef struct {
    char chunkId[4];
    int  chunkSize;
    char format[4];

    char subChunk1Id[4];
    int  subChunk1Size;
    short int audioFormat;
    short int numChannels;
    int sampleRate;
    int byteRate;
    short int blockAlign;
    short int bitsPerSample;

    char subChunk2Id[4];
    int  subChunk2Size;
} WaveHeader;

typedef struct {
    WaveHeader header;
    char* data;
    long long int index;
    long long int size;
    long long int nSamples;
} Wave;

Wave makeWave(int const sampleRate, short int const numChannels, short int const bitsPerSample);
void freeWave(Wave* wave);
void waveSetDuration(Wave* wave, const long long int milliseconds);
void waveAddSample16(Wave* wave, const int16_t* samples);
void waveToFile(Wave* wave, const char* filename);
