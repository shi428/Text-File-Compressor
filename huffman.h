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

typedef struct treende {
  unsigned char chr;
  struct treende *left;
  struct treende *right;
}TreeNode;

typedef struct {
  Node *root;
  TreeNode *root2;
}Tree;

typedef struct {
  unsigned char chr;
  unsigned char *bits;
  int size;
}code;


typedef struct listnode {
  unsigned char headerInstruction[3];
  struct listnode * next;
  struct listnode *prev;
  TreeNode *treenode;
}ListNode;

typedef struct {
  ListNode *head;
  ListNode *tail;
}List;

//encode
unsigned  readFile(char *filename);
element *copyToArray(priorityQueue *queue);
void copyToQueue(element *arr, priorityQueue *queue);
int countEncodedBits(char *filename, code *codes);
void writeEncodedText(char *filename, char *filename2, code *codes, int *compressedBytes);
void writeEncodedLength(char *filename, unsigned int bytes);

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
unsigned char binToDecimal(unsigned char *ptr);
//decode
int countBytes(char *filename);
unsigned int bytesInOriginalFile(char *filename, int *bytes);
unsigned char *readHeader(char *filename, int *bytes);
unsigned char *readData(char *filename, int bytes);
void parseBits(unsigned char *header, List *list);
List *createList();
void insert(List *list, ListNode *node, unsigned char *instruction,TreeNode *treeNode);
void push_back(List *list, unsigned char *leafNode, TreeNode *treeNode);
TreeNode* pop_node(List *list, ListNode *node);
Tree *buildTree(List *list);
void helper2(TreeNode *root, int counter, unsigned char *codeString, unsigned char bit, code *codes, int *ind);
int countLevels2(TreeNode *root, int counter, int max, int *numChars);
void decodeMessage(char *filename, unsigned char *data, Tree *tr, unsigned int numBytes);
void freeTree2(Tree *tr);
void freeNodes2(TreeNode *root);
void deleteList(List *list);
#endif
