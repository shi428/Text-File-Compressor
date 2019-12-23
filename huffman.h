#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <stdbool.h>
#include <string.h>
typedef struct {
  int freq;
  unsigned char chr;
  void *ptr;
}element;

typedef struct {
  element *arr;
  int front;
  int back;
  int size;
  int maxSize;
}priorityQueue;

typedef struct treenode {
  element data;
  struct treenode *left;
  struct treenode *right;
}Node;

typedef struct {
  Node *root;
}Tree;

typedef struct {
  unsigned char chr;
  unsigned char *bits;
  int size;
}code;

void readFile(char *filename);
element *copyToArray(priorityQueue *queue);
void copyToQueue(element *arr, priorityQueue *queue);
int countEncodedBits(char *filename, code *codes);
void writeEncodedText(char *filename, char *filename2, code *codes, int *compressedBytes);

void quickSort(element *arr, int start, int end);
int partition(element *arr, int start, int end);
void swap(void *a, void *b);

priorityQueue *newQueue();
void push(priorityQueue *queue, element data);
element pop(priorityQueue *queue);
void storeFrequencies(priorityQueue *queue);
bool isEmpty(priorityQueue *queue);
void freeQueue(priorityQueue *queue);
void print(priorityQueue *queue);
int comp(const void *a,const void *b);

Tree *buildHuffmanTree(priorityQueue *queue);
Node *newNode(element data);
element newElement(int freq, Node *ptr);
void freeTree(Tree *tr);
void freeNodes(Node *root);
code* getCodes(Tree *tr);
void helper(Node *root, int counter, unsigned char *codeString, unsigned char bit, code *codes, int *ind);
int countLevels(Node *root, int counter, int max, int *numChars);
void createHeader(Tree *tr, char *filename, int *compressedBytes);
int countNodes(Node *root, int counter);
void postOrder(Node *root, unsigned char *header, int *ind);

unsigned char *decToBinary(unsigned char chr);
#endif
