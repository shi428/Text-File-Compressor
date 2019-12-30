#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decompress.h"

void parseBits(unsigned char *header, List *list, unsigned short headerBits) {
  int i = 0;
  int length = headerBits;
  while (i < length) {
    if (header[i] == '1') {
      unsigned char chr = binToDecimal(&header[i+1]);
      unsigned char node[3];
      node[0] = header[i];
      node[1] = chr;
      node[2] = '\0';
      TreeNode *leaf  = createNode(chr);
      push_back(list, node, leaf);
      i += 9;
    }
    else {
      unsigned char node[2];
      node[0] = '0';
      node[1] = '\0';
      push_back(list, node, NULL);
      i++;
    }
  }
  ListNode *iterator = list->head;
  while (iterator) {
    printf("%s", iterator->headerInstruction);
    iterator = iterator->next;
  }
  printf("\n");
}

void decodeMessage(char *filename, unsigned char *data, Tree *tr, unsigned int numBytes) {
  FILE *fout = fopen(filename, "w");
  TreeNode *tn = tr->root;
  unsigned int counter = 0;
  int length = strlen((char *)data);
  for (int i = 0; i < length && counter < numBytes; i++) {
    if (!tn->left && !tn->right) {
      fputc(tn->chr, fout);
      tn = tr->root;
      counter++;
    }
    if (data[i] == '1') {
      tn = tn->right;
    }
    else {
      tn = tn->left;
    }
  }
  fclose(fout);
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

