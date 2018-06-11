#include "wavewriter.h"

int isBigEndian() {
    int test = 1;
    char *p = (char*)&test;

    return p[0] == 0;
}
void reverseEndianness(const long long int size, void* value){
    int i;
    char result[32];
    for( i=0; i<size; i+=1 ){
        result[i] = ((char*)value)[size-i-1];
    }
    for( i=0; i<size; i+=1 ){
        ((char*)value)[i] = result[i];
    }
}
void toBigEndian(const long long int size, void* value){
    char needsFix = !( (1 && isBigEndian()) || (0 && !isBigEndian()) );
    if( needsFix ){
        reverseEndianness(size,value);
    }
}
void toLittleEndian(const long long int size, void* value){
    char needsFix = !( (0 && isBigEndian()) || (1 && !isBigEndian()) );
    if( needsFix ){
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

  // All sizes for later:
  // chuckSize = 4 + (8 + subChunk1Size) + (8 + subChubk2Size)
  // subChunk1Size is constanst, i'm using 16 and staying with PCM
  // subChunk2Size = nSamples * nChannels * bitsPerSample/8
  // Whenever a sample is added:
  //    chunkSize += (nChannels * bitsPerSample/8)
  //    subChunk2Size += (nChannels * bitsPerSample/8)
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
  free( wave->data );
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

void waveAddSample16(Wave* wave, const int16_t* samples)
{
  int i;
  int sample16bit;
  char* sample;
  for( i=0; i<wave->header.numChannels; i+= 1){
    sample16bit = samples[i];
    toLittleEndian(2, (void*) &sample16bit);
    sample = (char*)&sample16bit;
    wave->data[wave->index + 0] = sample[0];
    wave->data[wave->index + 1] = sample[1];
    wave->index += 2;
  }
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
