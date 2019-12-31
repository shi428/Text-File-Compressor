#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "decompress.h"

int main(int argc, char *argv[]) {
  double time_spent = 0.0;
  clock_t begin = clock();
  int headerBytes = 0;
  unsigned short headerBits= countHeaderBits(argv[1], &headerBytes);
  unsigned char *header = readHeader(argv[1], &headerBytes, headerBits);
  List *list = createList();
  parseBits(header, list, headerBits);
  Tree *huffmanTree = buildTree(list);
  printf("Time elapsed is %f seconds\n", time_spent);
  code *codes = getCodes(huffmanTree);
/*  for (int i = 0; i < codes->size; i++) {
    printf("Character %c Code %s\n", codes[i].chr, codes[i].bits);
  }*/
  unsigned int bytes = bytesInOriginalFile(argv[1], &headerBytes);
  unsigned char *data = readData(argv[1], headerBytes);
  decodeMessage(argv[2], data, huffmanTree, bytes);
  bytes++;
  for (int i = 0; i < codes->size; i++) {
    free(codes[i].bits);
  }
  free(codes);
  free(header);
  free(data);
  freeTree(huffmanTree);
  clock_t end = clock();
  time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
  return EXIT_SUCCESS;
}
