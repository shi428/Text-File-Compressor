#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decompress.h"

Tree *buildTree(List *list) {
  Tree *huffmanTree = malloc(sizeof(Tree));
  ListNode *iterator = list->head;
  while (iterator) {
    ListNode *next = iterator->next;
    if (!strcmp((char *)iterator->headerInstruction,"0")) {
      ListNode *firstPrev = iterator->prev;
      ListNode *secondPrev = NULL;
      ListNode *thirdPrev = NULL;
      if (!firstPrev) {
        break;
      }
      secondPrev = firstPrev->prev;
      thirdPrev = secondPrev->prev;
      TreeNode *newNode = createNode(0);
   //   printf("%s %s\n", (char *)secondPrev->headerInstruction, (char *)firstPrev->headerInstruction);
      pop_node(list, iterator);
      newNode->right = pop_node(list,firstPrev);
      newNode->left = pop_node(list, secondPrev);
      insert(list, thirdPrev, NULL, newNode);
      huffmanTree->root = newNode;
    }
    iterator = next;
  }
  deleteList(list);
  return huffmanTree;
}

TreeNode *createNode(unsigned char chr) {
  TreeNode *newNode = malloc(sizeof(TreeNode));
  newNode->chr = chr;
  newNode->left = NULL;
  newNode->right = NULL;
  return newNode;
}

int countLevels(TreeNode *root, int counter, int max, int *numChars) {
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

void helper(TreeNode *root, int counter, unsigned char *codeString, unsigned char bit, code *codes, int *ind) {

  if (!root->left && !root->right) {
    if (counter >= 0) {
      codeString[counter] = bit;
    }
    codeString[counter + 1] = '\0';
    codes[*ind].chr =  root->chr;
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

void freeNodes(TreeNode *root) {
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
