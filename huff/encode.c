#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"
void writeEncodedLength(char *filename, unsigned int bytes) {
  FILE *fout = fopen(filename, "ab");
  fwrite(&bytes, 4, 1, fout);
  fclose(fout);
}

int countEncodedBits(char *filename, code *codes) {
  FILE *fin = fopen(filename, "r");
  if (!fin) {
    fprintf(stderr, "Failed to open %s\n", filename);
  }
  int ch;
  int numBits = 0;
  do {
    ch = fgetc(fin);
    if (ch != EOF) {
      for (int i = 0; i < codes->size; i++) {
        if (codes[i].chr == ch) {
          //          printf("Character %c Bits %s\n", codes[i].chr, codes[i].bits);
          numBits += strlen((char *)codes[i].bits);
        }
      }
    }
  } while (ch != EOF);
  fclose(fin);
  return numBits;
}

void writeEncodedText(char *filename, char *filename2, code *codes, int *compressedBytes) {
  unsigned char *bits = malloc(countEncodedBits(filename, codes) + 1);
  if (!bits) {
    printf("Failed to allocate memory!");
    return ;
  }
  FILE *fin = fopen(filename, "r");
  FILE *fout = fopen(filename2, "ab");
  if (!fin) {
    fprintf(stderr, "Failed to open %s for reading\n", filename);
    return ;
  }
  if (!fout) {
    fprintf(stderr, "Failed to open %s for writing\n", filename2);
    return ;
  }
  unsigned  int counter = 0;
  int ch;
  unsigned int length;
  do {
    ch = fgetc(fin);
    if (ch != EOF) {
      for (int i = 0; i < codes->size; i++) {
        if (ch == codes[i].chr) {
          length = strlen((char *)codes[i].bits);
          memcpy(bits + counter, codes[i].bits, length);
          counter += length;
        }
      }
  }
  } while (ch != EOF);
  bits[counter] = '\0';
  //  printf("%s\n", bits);
  unsigned char byte = 0;
  length = strlen((char *)bits);
  for (int i = 0; i < length; i++) {
    byte |= (bits[i] - '0') << (7 - (i % 8));
    if ((i+1) % 8 == 0) {
      fwrite(&byte, 1, 1, fout);
      byte = 0;
      (*compressedBytes)++;
    }
  }
  if (length % 8) {
    int rem = ((length / 8) + 1) - length;
    byte |= byte << rem;
    fwrite(&byte, 1, 1, fout);
    (*compressedBytes)++;
  }
  free(bits);
  fclose(fin);
  fclose(fout);
  printf("%s (compressed) contains %d bytes\n", filename2, *compressedBytes);
}

code* getCodes(Tree *tr) {
  //  printf("Tree contains: \n");
  int numChars = 0;
  int ind = 0;
  unsigned char *codeString = malloc(countLevels(tr->root, 0, 0, &numChars));
  code *codes = malloc(numChars * sizeof(code));
  codes->size = numChars;
  // printf("%d\n", numChars);
  helper(tr->root, -1, codeString, 0, codes, &ind);
  free(codeString);
  return codes;
}

void writeEncodedHeaderLength(FILE *fout, unsigned short bits) {
    fwrite(&bits, 2, 1, fout);
}

void createHeader(Tree *tr, char *filename, int *compressedBytes) {
  FILE *fout = fopen(filename, "wb");
  if (!fout) {
    fprintf(stderr, "Failed to open %s for writing\n", filename);
  }
  int nodes = countNodes(tr->root, 1);
  int numChars = 0;
  countLevels(tr->root, 0, 0, &numChars);
  unsigned char *header = malloc(numChars * 8 + nodes + 1);
  int ind = 0;
  postOrder(tr->root, header, &ind);
  header[ind] = '\0';
  // printf("\n");
  //  printf("%s\n", header);
  //int bitCount = 0;
  int length = strlen((char *)header);
  writeEncodedHeaderLength(fout, (unsigned short) length);
  //printf("There are %i bits in the header\n", length);
  unsigned char byte = 0;
  for (int i = 0; i < length; i++) {
    byte |= (header[i] - '0') << (7 - (i % 8));
    if ((i + 1) % 8 == 0) {
      (*compressedBytes)++;
      fwrite(&byte, 1, 1, fout);
      byte = 0;
    }
  }
  if (length % 8) {
    int remaining = ((length / 8) + 1) * 8 - length;
    byte |= byte << remaining;
    fwrite(&byte, 1, 1, fout);
    (*compressedBytes)++;
  }
  byte = '\n';
  fwrite(&byte, 1, 1, fout);
  (*compressedBytes)++;
    //printf("There are %d bytes stored in the header\n", *compressedBytes);
  fclose(fout);
  free(header);
}
