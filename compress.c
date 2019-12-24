#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "huffman.h"
int charFrequency[256] = {0};
int main(int argc, char *argv[]) {
  double time_spent = 0.0;
  clock_t begin = clock();
  int compressedBytes = 0;
  unsigned int numBytes = readFile(argv[1]);
  priorityQueue *frequencies = newQueue();
  storeFrequencies(frequencies);
//  print(frequencies);
  Tree *huffmanTr = buildHuffmanTree(frequencies);
  code *codes = getCodes(huffmanTr);
  for (int i = 0; i < codes->size; i++) {
    printf("Character %c Code %s\n", codes[i].chr, codes[i].bits);
    }
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
  int fuck = 0;
  do {
    ch = fgetc(fin);
    fuck++;
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

priorityQueue *newQueue() {
  priorityQueue *queue = malloc(sizeof(priorityQueue));
  queue->size = 0;
  queue->front = 0;
  queue->back = -1;
  queue->arr = NULL;
  queue->maxSize = 0;
  return queue;
}

void push(priorityQueue * queue, element data) {
  if (queue->size == queue->maxSize) {
    fprintf(stderr, "Cannot add any more data to the queue\n");
    return ;
  }
  queue->back = (queue->back + 1) % queue->maxSize;
  int back = queue->back;
  queue->arr[back] = data;
  queue->size++;
}

element pop(priorityQueue *queue) {
  if (isEmpty(queue)) {
    fprintf(stderr, "queue empty!\n");
    exit(EXIT_FAILURE);
  }
  int previousFront = queue->front;
  queue->front = (queue->front + 1) % queue->maxSize;
  queue->size--;
  return queue->arr[previousFront];
}

bool isEmpty(priorityQueue *queue) {
  return !queue->size;
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

int comp(const void *a, const void *b) {
  element *first = (element *)a;
  element *second = (element *)b;
  return first->freq - second->freq;
}

void freeQueue(priorityQueue *queue) {
  free(queue->arr);
  free(queue);
}

void print(priorityQueue *queue) {
  printf("Queue contains\n");
  int i = queue->front;
  for (int counter = 0; counter < queue->size; counter++) {
      printf("Character %c Frequency %d\n", queue->arr[i].chr, queue->arr[i].freq);
    i = (i + 1) % queue->maxSize;
  }
}

element newElement(int freq, Node *ptr) {
  element a;
  a.chr = 0;
  a.freq = freq;
  a.ptr = ptr;
  return a;
}

Node *newNode(element data) {
  Node *nde = malloc(sizeof(Node));
  nde->data = data;
  nde->left = NULL;
  nde->right = NULL;
  return nde;
}

element *copyToArray(priorityQueue * queue) {
  element *arr = malloc(queue->size * sizeof(element));
  int j = queue->front;
  for (int i = 0; i < queue->size; i++) {
    arr[i] = queue->arr[j];
    j = (j + 1 ) % queue->maxSize;
  }
  return arr;
}

void copyToQueue(element *arr, priorityQueue *queue) {
  int j = queue->front;
  for (int i = 0; i < queue->size; i++) {
    queue->arr[j] = arr[i];
    j = (j + 1) % queue->maxSize;
  }
}

Tree *buildHuffmanTree(priorityQueue *frequencies) {
  element *arr = copyToArray(frequencies);
  quickSort(arr, 0, frequencies->size - 1);
  copyToQueue(arr, frequencies);
  free(arr);
  Tree *huffmanTr = malloc(sizeof(Tree));
  element a, b;
  while (frequencies->size != 1) {
    a = pop(frequencies);
    b = pop(frequencies);
    huffmanTr->root = newNode(newElement(a.freq + b.freq, NULL));
    if (a.ptr && !b.ptr) {
      huffmanTr->root->left = a.ptr;
      huffmanTr->root->right = newNode(b);
    }
    else if (!a.ptr && b.ptr) {
      huffmanTr->root->left = newNode(a);
      huffmanTr->root->right = b.ptr;
    }
    else if (a.ptr && b.ptr) {
      huffmanTr->root->left = a.ptr;
      huffmanTr->root->right = b.ptr;
    }
    else {
      huffmanTr->root->left = newNode(a);
      huffmanTr->root->right = newNode(b);
    }
    push(frequencies, newElement(a.freq + b.freq, huffmanTr->root));
    arr = copyToArray(frequencies);
    quickSort(arr, 0, frequencies->size - 1);
    copyToQueue(arr, frequencies);
    free(arr);
  }
  return huffmanTr;
}

void freeNodes(Node *root) {
  if (!root) {
    return ;
  }
  freeNodes(root->left);
  freeNodes(root->right);
  free(root);
}

void freeTree(Tree *tr) {
  freeNodes(tr->root);
  free(tr);
}

int countLevels(Node *root, int counter, int max, int *numChars) {
  if (!root) {
    if (counter > max) {
      max = counter;
    }
    return max;
  }
  if (!root->left && !root->right) {
    (*numChars)++;
  }
  max = countLevels(root->left, counter + 1, max, numChars);
  max = countLevels(root->right, counter + 1, max, numChars);
  return max;
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

void helper(Node *root, int counter, unsigned char *codeString, unsigned char bit, code *codes, int *ind) {
  if (!root->left && !root->right) {
    if (counter >= 0) {
      codeString[counter] = bit;
    }
    codeString[counter + 1] = '\0';
    codes[*ind].chr =  root->data.chr;
    codes[*ind].bits = malloc(counter + 2);
    strcpy((char *)codes[(*ind)++].bits, (char *)codeString);
    return ;
  }
  if (counter >= 0) {
    codeString[counter] = bit;
  }
  helper(root->left, counter + 1, codeString, '0', codes, ind);
  helper(root->right, counter + 1, codeString, '1', codes, ind);
}

int countNodes(Node *root, int counter) {
  if (!root) {
    return counter;
  }
  counter = countNodes(root->left, counter + 1);
  counter = countNodes(root->right, counter + 1);
  return counter;
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
    printf("There are %d bytes stored in the header\n", *compressedBytes);
  fclose(fout);
  free(header);
}

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

void postOrder(Node *root, unsigned char *header, int *ind) {
  if (!root) {
    return ;
  }
  postOrder(root->left, header, ind);
  postOrder(root->right, header, ind);
  if (!root->left && !root->right) {
    header[(*ind)++] = '1';
    //   printf("1%c", root->data.chr);
    unsigned char *temp = decToBinary(root->data.chr);
    int length = strlen((char *)temp);
    memcpy(header + (*ind), temp, length);
    free(temp);
    (*ind) += length;
    return ;
  }
  header[(*ind)++] = '0';
  //  printf("0");
}

void swap(void *a,void *b) {
  element temp = *(element *)a;
  *(element *)a = *(element *)b;
  *(element *)b = temp;
}

int partition(element *arr, int start, int end) {
  int high = start;
  int low = start;
  while (high != end) {
    if (arr[high].freq < arr[end].freq) {
      swap(&arr[high], &arr[low]);
      low++;
    }
    high++;
  }
  swap(&arr[low], &arr[end]);
  return low;
}

void quickSort(element *arr, int start, int end) {
  if (start < end) {
    int pivot = partition(arr, start, end);
    quickSort(arr, start, pivot - 1);
    quickSort(arr, pivot + 1, end);
  }
}
