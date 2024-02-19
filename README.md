# My Heap 

Small quick project based on what we learnt in class. Allows the user to create a heap memory with a specified size and then allows to store data on that heap with matadata. Then allows those blocks to be deallocated and merged later. 

## Usage 

```c
// creates a heap with 256 allocated bytes
struct myheap* h = heapCreate(256);

// allocates memory on that heap
int* arr1 = myheapMalloc(h, sizeof(int)\*5);
int* arr2 = myheapMalloc(h, sizeof(int)\*10);

// data modification
arr1[0] = 1;
arr1[1] = 2;
arr1[2] = 3;

arr2[3] = 3;
arr2[6] = 2;
arr2[9] = 1;

// frees the data
myheapFree(h, arr1);
myheapFree(h, arr2);
```

## Useful Feature 

```c
printBlocks(h);
```

Shows the blocks allocated currently 

```
MY-HEAP: 4306812928 - size: 240 - start: 4306812944
block: 0 - 4306812944 - size: 40 used: 1
block: 1 - 4306812984 - size: 56 used: 1
block: 2 - 4306813040 - size: 144 used: 0
```
