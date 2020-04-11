#ifndef Q1_H_
#define Q1_H_

#include <stdbool.h>
#include <stdlib.h>

typedef struct node_t {
    int x;
    struct node_t *next;
} * Node;

typedef enum {
    SUCCESS = 0,
    MEMORY_ERROR,
    EMPTY_LIST,
    UNSORTED_LIST,
    NULL_ARGUMENT,
} ErrorCode;

int getListLength(Node list);
bool isListSorted(Node list);
void freeList(Node list);

ErrorCode mergeSortedLists(const Node list1, const Node list2, Node *merged_out);
#endif