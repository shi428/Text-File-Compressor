#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decompress.h"
int countBytes(char *filename) {
  FILE *fin = fopen(filename, "r");
  int ch;
  int counter = 0;
  do {
    ch = fgetc(fin);
    if (ch != EOF) {
      counter++;
    }
  }
  while (ch != EOF);
  fclose(fin);
  return counter;
}

unsigned short countHeaderBits(char *filename, int *bytes) {
  FILE *fin = fopen(filename, "r");
  unsigned short ret;
  fread(&ret, 2, 1, fin);
  (*bytes)+=2;
  fclose(fin);
  return ret;
}

unsigned int bytesInOriginalFile(char *filename, int *bytes) {
  FILE *fin = fopen(filename, "r");
  fseek(fin, *bytes, SEEK_SET);
  unsigned int numBytes;
  fread(&numBytes, 4, 1, fin);
  (*bytes)+= 4;
  fclose(fin);
  return numBytes;
}

unsigned char *readHeader(char *filename, int *bytes, unsigned short headerbits) {
  FILE *fin = fopen(filename, "r");
  int ch;
  unsigned short counter = 0;
  fseek(fin, *bytes, SEEK_SET);
  unsigned short headerBytes;
  printf("There are %i bits in the header\n", headerbits);
  if (headerbits % 8) {
    headerBytes = headerbits / 8 + 2;
  }
  else {
    headerBytes = headerbits /  8 + 1;
  }
  do {
    ch = fgetc(fin);
    counter++;
  }while (counter < headerBytes);
  unsigned char *bits = malloc(8 * counter + 1);
  fseek(fin, *bytes, SEEK_SET);
  counter = 0;
  int offset = 0;
  do {
    ch = fgetc(fin);
    unsigned char *temp = decToBinary((unsigned char)ch);
    int length = strlen((char *)temp);
    memcpy(bits + offset, temp,  length);
    offset += length;
    free(temp);
    counter++;
  } while (counter < headerBytes);
  bits[offset] = '\0';
  printf("%s\n", bits);
  (*bytes) += headerBytes;
  printf("There are %d bytes stored in the header\n", headerBytes);
  fclose(fin);
  return bits;
}

unsigned char *readData(char *filename, int bytes) {
  FILE *fin = fopen(filename, "r");
  fseek(fin, bytes, SEEK_SET);
  unsigned char *bits = malloc(8 * (countBytes(filename) - bytes) + 1);
  int ch;
  int offset = 0;
  do {
    ch = fgetc(fin);
    if (ch != EOF) {
      unsigned char *temp = decToBinary((unsigned char)ch);
      int length = strlen((char *)temp);
      memcpy(bits + offset, temp, length);
      offset += length;
      free(temp);
    }
  }while (ch != EOF);
  bits[offset] = '\0';
  fclose(fin);
  return bits;
}

