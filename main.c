#include <stdio.h>

#include "myheap.h"

int main(){
  struct myheap* h = heapCreate(256);
  int* arr1 = myheapMalloc(h, sizeof(int)*5);
  int* arr2 = myheapMalloc(h, sizeof(int)*10);

  arr1[0] = 1;
  arr1[1] = 2;
  arr1[2] = 3;

  arr2[3] = 3;
  arr2[6] = 2;
  arr2[9] = 1;

  printBlocks(h);
  myheapFree(h, arr1);
  myheapFree(h, arr2);

  printBlocks(h);
  return 0;
}
