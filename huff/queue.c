#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"
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

void print(priorityQueue *queue) {
  printf("Queue contains\n");
  int i = queue->front;
  for (int counter = 0; counter < queue->size; counter++) {
      printf("Character %c Frequency %d\n", queue->arr[i].chr, queue->arr[i].freq);
    i = (i + 1) % queue->maxSize;
  }
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

void freeQueue(priorityQueue *queue) {
  free(queue->arr);
  free(queue);
}
