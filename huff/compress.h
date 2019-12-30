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

//read
unsigned int readFile(char *filename);
void storeFrequencies(priorityQueue *frequencies);

//sort
void swap(void *a, void *b);
int partition(element *arr, int start, int end);
void quickSort(element *arr, int start, int end);

//queue
priorityQueue *newQueue();
void push(priorityQueue *queue, element data);
element pop(priorityQueue *queue);
void print(priorityQueue *queue);
element *copyToArray(priorityQueue *queue);
void copyToQueue(element *arr, priorityQueue *queue);
void freeQueue(priorityQueue *queue);

//tree
Tree *buildHuffmanTree(priorityQueue *frequencies);
Node *newNode(element data);
int countLevels(Node *root, int counter, int max, int *numChars);
void helper(Node *root, int counter, unsigned char *codeString, unsigned char bit, code *codes, int *ind);
int countNodes(Node *root, int counter);
void postOrder(Node *root, unsigned char *header, int *ind);
void freeNodes(Node *root);
void freeTree(Tree *tr);

//encode
void writeEncodedLength(char *filename, unsigned int bytes);
int countEncodedBits(char *fielname, code *codes);
void writeEncodedText(char *filename, char *filename2, code*codes, int *compressedBytes);
code *getCodes(Tree *Tr);
void writeEncodedHeaderLenghb(FILE *fout, unsigned short bits);
void createHeader(Tree *tr, char *filename, int *compresedBytes);

//misc
element newElement(int freq, Node *ptr);
unsigned char *decToBinary(unsigned char chr);
