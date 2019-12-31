#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"
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

Node *newNode(element data) {
  Node *nde = malloc(sizeof(Node));
  nde->data = data;
  nde->left = NULL;
  nde->right = NULL;
  return nde;
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

void postOrder(Node *root, unsigned char *header, int *ind) {
  if (!root) {
    return ;
  }
  postOrder(root->left, header, ind);
  postOrder(root->right, header, ind);
  if (!root->left && !root->right) {
    header[(*ind)++] = '1';
       //printf("1%c", root->data.chr);
    unsigned char *temp = decToBinary(root->data.chr);
    int length = strlen((char *)temp);
    memcpy(header + (*ind), temp, length);
    free(temp);
    (*ind) += length;
    return ;
  }
  header[(*ind)++] = '0';
    //printf("0");
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
