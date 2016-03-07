#include <stdio.h>
#include <unistd.h>

typedef struct block *tblock;
struct block {
    int size;
    tblock next;
    tblock prev;
    int free;
    int padding;
    void *ptr;
    char data[1];
};

static tblock firstBlock = NULL;

tblock findBlock(tblock *last, int size) {
    tblock block = firstBlock;
    tblock bestBlock = firstBlock;
    while(block) {
        if(block->free && block->size >= size) {
            if(!bestBlock->free)
                bestBlock = block;
            if(block->size >= bestBlock->size) {
                bestBlock = block;
            }
        }
        *last = block;
        block = block->next;
    }
    if(bestBlock->free)
        return bestBlock;
    else
        return NULL;
}

int align(int size) {
    if(size%8 == 0) return size;
    return ((size>>3)+1) << 3;
}

tblock newBlock(tblock last, int size) {
    tblock block;
    int s = align(size);
    block = sbrk(0);
    if(sbrk(24+s) == (void*)-1) {
        return NULL;
    }

    block->size = s;
    block->next = NULL;
    block->prev = last;
    block->ptr = block->data;

    if(last)
        last->next = block;
    block->free = 0;
    return block;
}

void *_malloc(int size) {
    tblock block, last;
    int s;
    s = align(size);
    if(firstBlock) {
        last = firstBlock;
        block = findBlock(&last, s);
        if(block) {
            block->free = 0;
        } else {
            block = newBlock(last, s);
            if(!block)
                return NULL;
        }
    } else {
        block = newBlock(NULL, s);
        if(!block)
            return NULL;
        firstBlock = block;
    }
    return block->data;
}

tblock getBlock(void *p) {
    char *tmp;
    tmp = p;
    return (p = tmp -= 24);
}

int validAddr(void *p) {
    if((tblock)p <= firstBlock || p >= sbrk(0))
        return 0;
    if(getBlock(p)->ptr == p)
        return 1;
    else
        return 0;
}

tblock mergeBlock(tblock block) {
    if(block->next && block->next->free) {
        block->size += 24 + block->next->size;
        block->next = block->next->next;
        if(block->next)
            block->next->prev = block;
        block->free = 1;
    }
    return block;
}

void free(void *p) {
    tblock block;
    if(validAddr(p)) {
        block = getBlock(p);
        block->free = 1;
        if(block->prev && block->prev->free)
            block = mergeBlock(block->prev);
        if(block->next)
            mergeBlock(block);
        else {
            if(block->prev)
                block->prev->prev = NULL;
            else
                firstBlock = NULL;
            brk(block);
        }
    }
}

void print() {
    tblock tmp = (tblock)firstBlock;
    int i = 0;
    while(tmp) {
        i++;
        printf("block = %d, free = %d, size = %d, value = %d\n", i, tmp->free, (int)tmp->size, *((int*)tmp->data));
        tmp =tmp->next;
    }
}

int main() {
    int *p;
    for(int i = 0; i < 100; i++) {
        p = _malloc(sizeof(int));
        if(i == 6 || i == 5)
            free(p);
        *p = i;
    }
    free(p);
    print();
    return 0;
}
