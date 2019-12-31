#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "compress.h"
int main(int argc, char *argv[]) {
  double time_spent = 0.0;
  clock_t begin = clock();
  if (argc == 1) {
    puts("Need input file!");
    return EXIT_FAILURE;
  }
  if (argc == 2) {
    puts("Need output file!");
    return EXIT_FAILURE;
  }
  int compressedBytes = 0;
  unsigned int numBytes = readFile(argv[1]);
  if (numBytes == EXIT_FAILURE) {
    return EXIT_FAILURE;
  }
  priorityQueue *frequencies = newQueue();
  storeFrequencies(frequencies);
//  print(frequencies);
  Tree *huffmanTr = buildHuffmanTree(frequencies);
  code *codes = getCodes(huffmanTr);
  /*for (int i = 0; i < codes->size; i++) {
    printf("Character %c Code %s\n", codes[i].chr, codes[i].bits);
    }*/
  createHeader(huffmanTr, argv[2], &compressedBytes);
  writeEncodedLength(argv[2], numBytes);
  writeEncodedText(argv[1], argv[2], codes, &compressedBytes);
  freeQueue(frequencies);
  freeTree(huffmanTr);
  for (int i = 0; i < codes->size; i++) {
    free(codes[i].bits);
  }
  free(codes);
  clock_t end = clock();
  time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Time elapsed is %f seconds\n", time_spent);
  return EXIT_SUCCESS;
}
