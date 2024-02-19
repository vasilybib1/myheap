#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "myheap.h"

#define HEADER_SIZE 8

struct myheap {
  long size;
  void *start;
};

static int blockIsInUse(void* blockStart) {
  return 1 & *((long*) blockStart);
}

static int getBlockSize(void* blockStart) {
  long* header = blockStart;
  return *header & 0xfffffffffffffffe;
}

static int getPayloadSize(void* blockStart) {
  return getBlockSize(blockStart) - HEADER_SIZE * 2;
}

static void* getBlockStart(void* payload) {
  return payload - HEADER_SIZE;
}

static void* getPayload(void* blockStart) {
  return blockStart + HEADER_SIZE;
}

static void setBlockHeader(void* blockStart, int blockSize, int inUse) {
  long headerValue = blockSize | inUse;
  long* headerPosition = blockStart;
  long* trailerPosition = blockStart;
  *headerPosition = headerValue;
  *trailerPosition = headerValue;
}

static void* getNextBlock(void* blockStart) {
  return blockStart + getBlockSize(blockStart);
}

static void* getPreviousBlock(void* blockStart) {
  return blockStart - getBlockSize(blockStart - HEADER_SIZE);
}

static int isFirstBlock(struct myheap* h, void* blockStart) {
  return blockStart == h->start;
}

static int isLastBlock(struct myheap* h, void* blockStart) {
  return getNextBlock(blockStart) == h->start + h->size;
}

static int isWithinHeapRange(struct myheap* h, void* addr) {
  return addr >= h->start && addr < h->start + h->size;
}

static void* coalesce(struct myheap* h, void* firstBlockStart) {
  void* nextBlock = getNextBlock(firstBlockStart);
  if(isLastBlock(h, nextBlock))
    return NULL;
  if(blockIsInUse(firstBlockStart) == 0) {
    int size = getBlockSize(firstBlockStart) + getBlockSize(nextBlock);
    setBlockHeader(firstBlockStart, size, 0);
    return firstBlockStart;
  }
  return NULL;
}

static int getSizeToAllocate(int userSize) {
  if(userSize % HEADER_SIZE == 0)
    return userSize + HEADER_SIZE*2;
  return userSize + (HEADER_SIZE-(userSize % HEADER_SIZE)) + HEADER_SIZE*2;
}

static void* splitAndMarkUsed(struct myheap* h, void* blockStart, int neededSize) {
  int size = getBlockSize(blockStart);
  if(size - neededSize >= HEADER_SIZE*3) {
    setBlockHeader(blockStart, neededSize, 1);
    setBlockHeader(getNextBlock(blockStart), size - neededSize, 0);
  } else {
    setBlockHeader(blockStart, getBlockSize(blockStart), 1);
  }
  return getPayload(blockStart);
}

struct myheap* heapCreate(unsigned int size) {
  void* heapStart = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if(heapStart == (void*)-1) 
    return NULL;

  struct myheap* h = heapStart;
  h->size = size - sizeof(struct myheap);
  h->start = heapStart + sizeof(struct myheap);

  setBlockHeader(h->start, h->size, 0);
  return h;
}

void myheapFree(struct myheap* h, void* payload) {
  void* block = getBlockStart(payload);
  setBlockHeader(block, getBlockSize(block), 0);

  while(1){
    if(coalesce(h, block) == NULL)
      break;
    if(isLastBlock(h, block))
      break;
  }

  if(!isFirstBlock(h, block))
    if(!blockIsInUse(getPreviousBlock(block)))
      coalesce(h, getPreviousBlock(block));
}

void* myheapMalloc(struct myheap* h, unsigned int userSize) {
  void* block = h->start;

  while(1) {
    if(blockIsInUse(block) == 0 && getBlockSize(block) >= getSizeToAllocate(userSize))
      return splitAndMarkUsed(h, block, getSizeToAllocate(userSize));
    else if(isLastBlock(h, block))
      break;
    else 
      block = getNextBlock(block);
  }

  return NULL;
}

void printBlocks(struct myheap* h) {
  printf("MY-HEAP: %ld - size: %ld - start: %ld\n", (long)h, h->size, (long)h->start);
  void* current = h->start;
  int count = 0;
  while(1){
    printf("block: %d - %ld - size: %d used: %d\n", 
        count, (long)current, getBlockSize(current), blockIsInUse(current));
    count++;
    if(isLastBlock(h, current))
      break;
    current = getNextBlock(current);
  }
}

