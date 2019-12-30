#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decompress.h"

unsigned char *decToBinary(unsigned char chr) {
  int counter = 0;
  unsigned char *bits = malloc(9);
  unsigned char temp = chr;
  while (temp) {
    temp /= 2;
    counter++;
  }
  int remainingBits = 8 - counter;
  for (int i = 0; i < remainingBits; i++) {
    bits[i] = '0';
  }
  counter = 0;
  while (chr) {
    bits[remainingBits + counter] = (chr % 2) + '0';
    chr /= 2;
    counter++;
  }
  for (int i = 0; i < counter/2; i++) {
    temp = bits[remainingBits + i];
    bits[remainingBits + i] = bits[remainingBits + counter - i - 1];
    bits[remainingBits + counter - i - 1] =temp;
  }
  bits[remainingBits + counter] = '\0';
  return bits;
}

unsigned char binToDecimal(unsigned char *ptr) {
  unsigned char ans = 0;
  for (int i = 0; i < 8; i++) {
    ans |= (ptr[i]-'0') << (7 - i);
  }
  return ans;
}
