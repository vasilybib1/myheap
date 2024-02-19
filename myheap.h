#ifndef MYHEAP_H
#define MYHEAP_H

struct myheap;
struct myheap* heapCreate(unsigned int size);
void myheapFree(struct myheap* h, void* payload);
void* myheapMalloc(struct myheap* h, unsigned int size);
void printBlocks(struct myheap* h);

#endif
