#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

#define false 0
#define true 1

char _heap[MEMSIZE] = {0};
TNode *_memlist = NULL; // To maintain information about length

void initializeMemlist();

// Do not change this. Used by the test harness.
// You may however use this function in your code if necessary.
long get_index(void *ptr) {
    if (ptr == NULL)
        return -1;
    else
        return (long) ((char *) ptr - &_heap[0]);
}

void print_node(TNode *node) {
    printf("Status: %s Start index: %d Length: %d\n", node->pdata->allocated ? "ALLOCATED" : "FREE", node->key,
           node->pdata->len);
}

void print_memlist() {
    process_list(_memlist, print_node);
}

TNode *make_memnode(unsigned int start, unsigned int len, int allocated) {
    TNode *node = malloc(sizeof(TNode));
    node->key = start;
    TData *data = (TData *) malloc(sizeof(TData));
    data->len = len;
    data->allocated = allocated;
    node->pdata = data;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

void initializeMemlist() {
    TNode *node = make_memnode(0, MEMSIZE, false);
    insert_node(&_memlist, node, ASCENDING);
}


TNode *selectWf(size_t size) {
    TNode *best = NULL;
    unsigned int diff = 0;
    for(TNode *iter = _memlist; iter != NULL; iter = iter->next){
        if (iter->pdata->len < size || (iter->pdata->allocated)) {
            continue;
        }
        unsigned int newDiff = iter->pdata->len - size;
        if(best != NULL && diff > newDiff){
            continue;
        }
        best = iter;
        diff = newDiff;
    }
    return best;
}

// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size) {
    if (_memlist == NULL) {
        initializeMemlist();
    }
    //iterate and choose frame
    TNode *choice = selectWf(size);

    if (choice == NULL) {
//        return NULL;
    }
    //assume frameSize >= size
    //break frame into 2 pcs (if not perfect fit)
    if (choice->pdata->len > size) {
        unsigned int start = choice->key;
        unsigned int remainStart = start + size;
        unsigned int remainLen = choice->pdata->len - size;
        TNode *n1 = make_memnode(start, size, false);
        TNode *n2 = make_memnode(remainStart, remainLen, false);

        delete_node(&_memlist, choice);
        insert_node(&_memlist, n1, 0);
        insert_node(&_memlist, n2, 0);
        choice = find_node(_memlist, start);
    }

    //allocate first piece and return
    choice->pdata->allocated = true;
    return &(_heap[choice->key]);
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
    unsigned int index = get_index(ptr);
    if (index < 0 || index >= MEMSIZE) {
        return;
    }
    TNode *node = find_node(_memlist, index);
    if (node == NULL) {
        return;
    }
    if (!node->pdata->allocated) {
        return;
    }
    node->pdata->allocated = false;
    //merge with previous
    TNode *prev = node->prev;
    if (prev != NULL && !(prev->pdata->allocated)) {
        unsigned int prevStart = prev->key;
        unsigned int prevLen = prev->pdata->len;
        merge_node(_memlist, node, 0);
        node->key = prevStart;
        node->pdata->len += prevLen;
    }
    //merge with next
    TNode *next = node->next;
    if (next != NULL && !(next->pdata->allocated)) {
        unsigned int nextLen = next->pdata->len;
        merge_node(_memlist, node, 1);
        node->pdata->len += nextLen;
    }
}

