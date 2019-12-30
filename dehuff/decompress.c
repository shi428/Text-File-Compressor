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
  for (int i = 0; i < codes->size; i++) {
    printf("Character %c Code %s\n", codes[i].chr, codes[i].bits);
  }
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

/*int countBytes(char *filename) {
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
}*/

//unsigned short countHeaderBits(char *filename, int *bytes) {
//  FILE *fin = fopen(filename, "r");
//  unsigned short ret;
//  fread(&ret, 2, 1, fin);
//  (*bytes)+=2;
//  fclose(fin);
//  return ret;
//}

//unsigned int bytesInOriginalFile(char *filename, int *bytes) {
//  FILE *fin = fopen(filename, "r");
//  fseek(fin, *bytes, SEEK_SET);
//  unsigned int numBytes;
//  fread(&numBytes, 4, 1, fin);
//  (*bytes)+= 4;
//  fclose(fin);
//  return numBytes;
//}

//unsigned char *readHeader(char *filename, int *bytes, unsigned short headerbits) {
//  FILE *fin = fopen(filename, "r");
//  int ch;
//  unsigned short counter = 0;
//  fseek(fin, *bytes, SEEK_SET);
//  unsigned short headerBytes;
//  printf("There are %i bits in the header\n", headerbits);
//  if (headerbits % 8) {
//    headerBytes = headerbits / 8 + 2;
//  }
//  else {
//    headerBytes = headerbits /  8 + 1;
//  }
//  do {
//    ch = fgetc(fin);
//    counter++;
//  }while (counter < headerBytes);
//  unsigned char *bits = malloc(8 * counter + 1);
//  fseek(fin, *bytes, SEEK_SET);
//  counter = 0;
//  int offset = 0;
//  do {
//    ch = fgetc(fin);
//    unsigned char *temp = decToBinary((unsigned char)ch);
//    int length = strlen((char *)temp);
//    memcpy(bits + offset, temp,  length);
//    offset += length;
//    free(temp);
//    counter++;
//  } while (counter < headerBytes);
//  bits[offset] = '\0';
//  printf("%s\n", bits);
//  (*bytes) += headerBytes;
//  printf("There are %d bytes stored in the header\n", headerBytes);
//  fclose(fin);
//  return bits;
//}

//unsigned char *readData(char *filename, int bytes) {
//  FILE *fin = fopen(filename, "r");
//  fseek(fin, bytes, SEEK_SET);
//  unsigned char *bits = malloc(8 * (countBytes(filename) - bytes) + 1);
//  int ch;
//  int offset = 0;
//  do {
//    ch = fgetc(fin);
//    if (ch != EOF) {
//      unsigned char *temp = decToBinary((unsigned char)ch);
//      int length = strlen((char *)temp);
//      memcpy(bits + offset, temp, length);
//      offset += length;
//      free(temp);
//    }
//  }while (ch != EOF);
//  bits[offset] = '\0';
//  fclose(fin);
//  return bits;
//}

//unsigned char *decToBinary(unsigned char chr) {
//  int counter = 0;
//  unsigned char *bits = malloc(9);
//  unsigned char temp = chr;
//  while (temp) {
//    temp /= 2;
//    counter++;
//  }
//  int remainingBits = 8 - counter;
//  for (int i = 0; i < remainingBits; i++) {
//    bits[i] = '0';
//  }
//  counter = 0;
//  while (chr) {
//    bits[remainingBits + counter] = (chr % 2) + '0';
//    chr /= 2;
//    counter++;
//  }
//  for (int i = 0; i < counter/2; i++) {
//    temp = bits[remainingBits + i];
//    bits[remainingBits + i] = bits[remainingBits + counter - i - 1];
//    bits[remainingBits + counter - i - 1] =temp;
//  }
//  bits[remainingBits + counter] = '\0';
//  return bits;
//}

//List *createList() {
//  List *ret = malloc(sizeof(List));
//  ret->head = NULL;
//  ret->tail = NULL;
//  return ret;
//}

//unsigned char binToDecimal(unsigned char *ptr) {
//  unsigned char ans = 0;
//  for (int i = 0; i < 8; i++) {
//    ans |= (ptr[i]-'0') << (7 - i);
//  }
//  return ans;
//}

//TreeNode *createNode(unsigned char chr) {
//  TreeNode *newNode = malloc(sizeof(TreeNode));
//  newNode->chr = chr;
//  newNode->left = NULL;
//  newNode->right = NULL;
//  return newNode;
//}

//void parseBits(unsigned char *header, List *list, unsigned short headerBits) {
//  int i = 0;
//  int length = headerBits;
//  while (i < length) {
//    if (header[i] == '1') {
//      unsigned char chr = binToDecimal(&header[i+1]);
//      unsigned char node[3];
//      node[0] = header[i];
//      node[1] = chr;
//      node[2] = '\0';
//      TreeNode *leaf  = createNode(chr);
//      push_back(list, node, leaf);
//      i += 9;
//    }
//    else {
//      unsigned char node[2];
//      node[0] = '0';
//      node[1] = '\0';
//      push_back(list, node, NULL);
//      i++;
//    }
//  }
//  ListNode *iterator = list->head;
//  while (iterator) {
//    printf("%s", iterator->headerInstruction);
//    iterator = iterator->next;
//  }
//  printf("\n");
//}

//TreeNode* pop_node(List *list, ListNode *node) {
//  TreeNode *ret = node->treenode;
//  if (node == list->head) {
//    if (list->head == list->tail) {
//      list->head = NULL;
//      list->tail = NULL;
//      free(node);
//      return ret;
//    }
//    list->head = node->next;
//    list->head->prev = NULL;
//    free(node);
//    return ret;
//  }
//  else if (node == list->tail) {
//    list->tail = list->tail->prev;
//    list->tail->next = NULL;
//    free(node);
//    return ret;
//  }
//  ListNode *iterator = list->head;
//  while (iterator) {
//    if (iterator->next == node) {
//      ListNode *next = node->next;
//      iterator->next = next;
//      next->prev = iterator;
//      free(node);
//    }
//    iterator = iterator->next;
//  }
//  return ret;
//}

//void push_back(List *list, unsigned char *instruction, TreeNode *treeNode) {
//  if (!list->head) {
//    list->head = malloc(sizeof(ListNode));
//    if (instruction) {
//      strcpy((char *)list->head->headerInstruction, (char *)instruction);
//    }
//    list->head->treenode = treeNode;
//    list->head->next = NULL;
//    list->head->prev = NULL;
//    list->tail = list->head;
//  }
//  else {
//    ListNode *prev = list->tail;
//    list->tail = malloc(sizeof(ListNode));
//    if (instruction) {
//      strcpy((char *)list->tail->headerInstruction, (char *)instruction);
//    }
//    list->tail->treenode = treeNode;
//    list->tail->next = NULL;
//    prev->next = list->tail;
//    list->tail->prev = prev;
//  }
//}

//void insert(List *list, ListNode *node, unsigned char *instruction, TreeNode *treeNode) {
//  ListNode *newNode = malloc(sizeof(ListNode));
//  if (instruction) {
//    strcpy((char *)newNode->headerInstruction, (char *)instruction);
//  }
//  newNode->treenode = treeNode;
//  if (!node) {
//    ListNode *next = list->head;
//    list->head = newNode;
//    if (next) {
//      list->head->next = next;
//      next->prev = list->head;
//    }
//    else {
//      list->head->next = NULL;
//    }
//  }
//  ListNode *iterator = list->head;
//  while (iterator) {
//    if (iterator == node) {
//      ListNode *next = iterator->next;
//      node->next= newNode;
//      newNode->prev = node;
//      newNode->next = next;
//      next->prev = newNode;
//    }
//    iterator = iterator->next;
//  }
//}

//Tree *buildTree(List *list) {
//  Tree *huffmanTree = malloc(sizeof(Tree));
//  ListNode *iterator = list->head;
//  while (iterator) {
//    ListNode *next = iterator->next;
//    if (!strcmp((char *)iterator->headerInstruction,"0")) {
//      ListNode *firstPrev = iterator->prev;
//      ListNode *secondPrev = NULL;
//      ListNode *thirdPrev = NULL;
//      if (!firstPrev) {
//        break;
//      }
//      secondPrev = firstPrev->prev;
//      thirdPrev = secondPrev->prev;
//      TreeNode *newNode = createNode(0);
//   //   printf("%s %s\n", (char *)secondPrev->headerInstruction, (char *)firstPrev->headerInstruction);
//      pop_node(list, iterator);
//      newNode->right = pop_node(list,firstPrev);
//      newNode->left = pop_node(list, secondPrev);
//      insert(list, thirdPrev, NULL, newNode);
//      huffmanTree->root2 = newNode;
//    }
//    iterator = next;
//  }
//  deleteList(list);
//  return huffmanTree;
//}

//int countLevels2(TreeNode *root, int counter, int max, int *numChars) {
//  if (!root) {
//    if (counter > max) {
//      max = counter;
//    }
//    return max;
//  }
//  if (!root->left && !root->right) {
//    (*numChars)++;
//  }
//  max = countLevels2(root->left, counter + 1, max, numChars);
//  max = countLevels2(root->right, counter + 1, max, numChars);
//  return max;
//}

//code* getCodes(Tree *tr) {
//  //  printf("Tree contains: \n");
//  int numChars = 0;
//  int ind = 0;
//  unsigned char *codeString = malloc(countLevels2(tr->root2, 0, 0, &numChars));
//  code *codes = malloc(numChars * sizeof(code));
//  codes->size = numChars;
//  // printf("%d\n", numChars);
//  helper2(tr->root2, -1, codeString, 0, codes, &ind);
//  free(codeString);
//  return codes;
//}

//void helper2(TreeNode *root, int counter, unsigned char *codeString, unsigned char bit, code *codes, int *ind) {
//  if (!root->left && !root->right) {
//    if (counter >= 0) {
//      codeString[counter] = bit;
//    }
//    codeString[counter + 1] = '\0';
//    codes[*ind].chr =  root->chr;
//    codes[*ind].bits = malloc(counter + 2);
//    strcpy((char *)codes[(*ind)++].bits, (char *)codeString);
//    return ;
//  }
//  if (counter >= 0) {
//    codeString[counter] = bit;
//  }
//  helper2(root->left, counter + 1, codeString, '0', codes, ind);
//  helper2(root->right, counter + 1, codeString, '1', codes, ind);
//}

//void decodeMessage(char *filename, unsigned char *data, Tree *tr, unsigned int numBytes) {
//  FILE *fout = fopen(filename, "w");
//  TreeNode *tn = tr->root2;
//  unsigned int counter = 0;
//  int length = strlen((char *)data);
//  for (int i = 0; i < length && counter < numBytes; i++) {
//    if (!tn->left && !tn->right) {
//      fputc(tn->chr, fout);
//      tn = tr->root2;
//      counter++;
//    }
//    if (data[i] == '1') {
//      tn = tn->right;
//    }
//    else {
//      tn = tn->left;
//    }
//  }
//  fclose(fout);
//}

//void freeNodes2(TreeNode *root) {
//  if (!root) {
//    return ;
//  }
//  freeNodes2(root->left);
//  freeNodes2(root->right);
//  free(root);
//}
//
//void freeTree2(Tree *tr) {
//  freeNodes2(tr->root2);
//  free(tr);
//}

//void deleteList(List *list) {
//  ListNode *iterator = list->head;
//  while (iterator) {
//    ListNode *next = iterator->next;
//    free(iterator);
//    iterator = next;
//  }
//  free(list);
//}
