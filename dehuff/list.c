#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decompress.h"

List *createList() {
  List *ret = malloc(sizeof(List));
  ret->head = NULL;
  ret->tail = NULL;
  return ret;
}

void push_back(List *list, unsigned char *instruction, TreeNode *treeNode) {
  if (!list->head) {
    list->head = malloc(sizeof(ListNode));
    if (instruction) {
      strcpy((char *)list->head->headerInstruction, (char *)instruction);
    }
    list->head->treenode = treeNode;
    list->head->next = NULL;
    list->head->prev = NULL;
    list->tail = list->head;
  }
  else {
    ListNode *prev = list->tail;
    list->tail = malloc(sizeof(ListNode));
    if (instruction) {
      strcpy((char *)list->tail->headerInstruction, (char *)instruction);
    }
    list->tail->treenode = treeNode;
    list->tail->next = NULL;
    prev->next = list->tail;
    list->tail->prev = prev;
  }
}

void insert(List *list, ListNode *node, unsigned char *instruction, TreeNode *treeNode) {
  ListNode *newNode = malloc(sizeof(ListNode));
  if (instruction) {
    strcpy((char *)newNode->headerInstruction, (char *)instruction);
  }
  newNode->treenode = treeNode;
  if (!node) {
    ListNode *next = list->head;
    list->head = newNode;
    if (next) {
      list->head->next = next;
      next->prev = list->head;
    }
    else {
      list->head->next = NULL;
    }
  }
  ListNode *iterator = list->head;
  while (iterator) {
    if (iterator == node) {
      ListNode *next = iterator->next;
      node->next= newNode;
      newNode->prev = node;
      newNode->next = next;
      next->prev = newNode;
    }
    iterator = iterator->next;
  }
}

TreeNode* pop_node(List *list, ListNode *node) {
  TreeNode *ret = node->treenode;
  if (node == list->head) {
    if (list->head == list->tail) {
      list->head = NULL;
      list->tail = NULL;
      free(node);
      return ret;
    }
    list->head = node->next;
    list->head->prev = NULL;
    free(node);
    return ret;
  }
  else if (node == list->tail) {
    list->tail = list->tail->prev;
    list->tail->next = NULL;
    free(node);
    return ret;
  }
  ListNode *iterator = list->head;
  while (iterator) {
    if (iterator->next == node) {
      ListNode *next = node->next;
      iterator->next = next;
      next->prev = iterator;
      free(node);
    }
    iterator = iterator->next;
  }
  return ret;
}

void deleteList(List *list) {
  ListNode *iterator = list->head;
  while (iterator) {
    ListNode *next = iterator->next;
    free(iterator);
    iterator = next;
  }
  free(list);
}
