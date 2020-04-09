#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "q1.h"

#define LIST_AS_STR_MAX_SIZE 3000
#define NODE_AS_STR_MAX_SIZE 50
#define TESTS_COUNT 100000
#define PRINT_LIST(list)                       \
    {                                          \
        Node list_tmp = list;                  \
        printf(#list ":\n\t");                 \
        if (list_tmp == NULL) {                \
            printf("NULL\n");                  \
        } else {                               \
            while (list_tmp != NULL) {         \
                printf("%.2d->", list_tmp->x); \
                list_tmp = list_tmp->next;     \
            }                                  \
            printf("NULL\n");                  \
        }                                      \
    }

Node copyNode(Node n) {
    Node newNode = malloc(sizeof(n));
    newNode->x = n->x;
    return newNode;
}

Node SortedMerge(Node a, Node b) {
    Node result = NULL; /* Base cases */
    if (a == NULL)
        return (b);
    else if (b == NULL)
        return (a); /* Pick either a or b, and recur */
    if (a->x <= b->x) {
        result = copyNode(a);
        result->next = SortedMerge(a->next, b);
    } else {
        result = copyNode(b);
        result->next = SortedMerge(a, b->next);
    }
    return (result);
}
///// Hello 
char *listToString(Node list) {
    char *str = malloc(LIST_AS_STR_MAX_SIZE);
    assert(str != NULL);
    str[0] = '\0';
    char buff[NODE_AS_STR_MAX_SIZE];

    while (list != NULL) {
        sprintf(buff, "%d->", list->x);
        strncat(str, buff, NODE_AS_STR_MAX_SIZE);
        list = list->next;
    }
    strcat(str, "NULL");
    return str;
}

bool areListsEqual(Node list1, Node list2) {
    while (list1 != NULL || list2 != NULL) {
        if (list1 == NULL || list2 == NULL || list1->x != list2->x) {
            return false;
        }
        list1 = list1->next;
        list2 = list2->next;
    }
    return true;
}

void testOnce() {
    Node lists[] = {malloc(sizeof(*lists[0])), malloc(sizeof(*lists[1]))};
    for (int i = 0; i < 2; i++) {
        Node curr = lists[i];
        int count = rand() % 10;  // Random number between 0 and 11
        int tmp_value;
        while (count > 0) {
            curr->next = malloc(sizeof(*lists[i]));
            tmp_value = rand() % 25;
            tmp_value = tmp_value ? (rand() % 5) : (-1 * tmp_value);
            curr->next->x =
                curr->x +
                tmp_value;  // Generate a series which is likely rising
            curr = curr->next;
            count--;
        }
        curr->next = NULL;
        curr = lists[i];
        lists[i] = lists[i]->next;
        free(curr);
    }

    char *list1_before = listToString(lists[0]);
    char *list2_before = listToString(lists[1]);

    Node mergedActual, mergedExpected = NULL;
    mergeSortedLists(lists[0], lists[1], &mergedActual);
    char *list1_after = listToString(lists[0]);
    char *list2_after = listToString(lists[1]);
    if (lists[0] && lists[1]) {
        mergedExpected = SortedMerge(lists[0], lists[1]);
    }

    if (!areListsEqual(mergedActual, mergedExpected) ||
        strcmp(list1_before, list1_after) ||
        strcmp(list2_before, list2_after)) {
        fprintf(stderr, "\n\n======= Lists not equal ========\n");

        fprintf(stderr, "List1 before:\n");
        fprintf(stderr, "\t%s\n", list1_before);
        fprintf(stderr, "List2 before:\n");
        fprintf(stderr, "\t%s\n", list2_before);

        fprintf(stderr, "List1 after:\n");
        fprintf(stderr, "\t%s\n", listToString(lists[0]));
        fprintf(stderr, "List2 after:\n");
        fprintf(stderr, "\t%s\n", listToString(lists[1]));

        fprintf(stderr, "Merged expected:\n");
        fprintf(stderr, "\t%s\n", listToString(mergedExpected));
        fprintf(stderr, "Merged actual:\n");
        fprintf(stderr, "\t%s\n", listToString(mergedActual));

    }

    freeList(mergedActual);
    freeList(mergedExpected);
    freeList(lists[0]);
    freeList(lists[1]);
}

int main() {
    srand(time(0));
    for (int i = 0; i < TESTS_COUNT; i++) {
        testOnce();
    }
}