#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"

int charFrequency[256] = {0};

unsigned int readFile(char *filename) {
  FILE *fin = fopen(filename, "r");
  if (!fin) {
    fprintf(stderr, "Failed to open %s\n", filename);
    return EXIT_FAILURE;;
  }
  int ch;
  unsigned int bytes = 0;
  do {
    ch = fgetc(fin);
    //putchar(ch);
    if (ch != EOF) {
      charFrequency[ch]++;
      bytes++;
    }
  } while (ch != EOF);
  printf("%s contains %d bytes\n", filename, bytes);
  fclose(fin);
  return bytes;
}

void storeFrequencies(priorityQueue *frequencies) {
  int counter = 0;
  for (int i = 0; i < 256; i++) {
    if (charFrequency[i]) {
      counter++;
    }
  }
  frequencies->arr = malloc(sizeof(element) * counter);
  frequencies->maxSize = counter;
  for (int i = 0; i < 256; i++) {
    if (charFrequency[i]) {
      element data;
      data.freq = charFrequency[i];
      data.chr = i;
      data.ptr = NULL;
      push(frequencies, data);
    }
  }
}
