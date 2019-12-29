#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"

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
