//structures
typedef struct treende {
  unsigned char chr;
  struct treende *left;
  struct treende *right;
}TreeNode;

typedef struct {
  TreeNode *root;
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
//read
int countBytes(char *filename);
unsigned short countHeaderBits(char *filename, int *bytes);
unsigned int bytesInOriginalFile(char *filename, int *bytes);
unsigned char *readHeader(char *filename, int *bytes, unsigned short headerbits);
unsigned char *readData(char *filename, int bytes);

//misc
unsigned char *decToBinary(unsigned char chr);
unsigned char binToDecimal(unsigned char *ptr);

//list
List *createList();
void push_back(List *list, unsigned char *instruction, TreeNode *treeNode);
void insert(List *list, ListNode *node, unsigned char *instruction, TreeNode *treeNode);
TreeNode *pop_node(List *list, ListNode *node);
void deleteList(List *list);
void printList(List *list);

//tree
Tree *buildTree(List *list);
TreeNode *createNode(unsigned char chr);
int countLevels(TreeNode *root, int counter, int max, int *numChars);
void helper(TreeNode *root, int counter, unsigned char *codeSTring, unsigned char bit, code *codes, int *ind);
void freeNodes(TreeNode *root);
void freeTree(Tree *tr);

//decode
void parseBits(unsigned char *header, List *list, unsigned short headerBits);
code *getCodes(Tree *tr);
void decodeMessage(char *filename, unsigned char *data, Tree *tr, unsigned int numBytes);
