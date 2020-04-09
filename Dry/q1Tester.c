#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "q1.h"

#define LIST_AS_STR_MAX_SIZE 3000
#define NODE_AS_STR_MAX_SIZE 50
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
    Node newNode = malloc(sizeof(*n));
    newNode->x = n->x;
    return newNode;
}

int getListLength(Node list) {
    int count = 0;

    while (list) {
        count++;
    }
    return count;
}

bool isListSorted(Node list) {
    if (list == NULL) {
        return true;
    }

    int prev = list->x;
    list = list->next;

    while (list != NULL) {
        if (prev > list->x) {
            return false;
        }
        prev = list->x;
        list = list->next;
    }

    return true;
}

Node copyList(Node list) {
    Node head = malloc(sizeof(*head)), curr = head;
    head->next = NULL;

    while (list) {
        curr->next = malloc(sizeof(*curr));
        curr = curr->next;
        curr->next = NULL;
        curr->x = list->x;
        list = list->next;
    }

    if (curr == head) {
        free(head);
        return NULL;
    }
    curr = head->next;
    free(head);

    return curr;
}

Node sortedMergeSol(Node a, Node b) {
    Node result = NULL; /* Base cases */
    if (a == NULL)
        return copyList(b);
    else if (b == NULL)
        return copyList(a); /* Pick either a or b, and recur */
    if (a->x <= b->x) {
        result = copyNode(a);
        result->next = sortedMergeSol(a->next, b);
    } else {
        result = copyNode(b);
        result->next = sortedMergeSol(a, b->next);
    }
    return (result);
}

char *listToString(Node list) {
    char *str = malloc(sizeof(char) * LIST_AS_STR_MAX_SIZE);
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

bool testOnce(int test_num) {
    Node lists[] = {malloc(sizeof(*lists[0])), malloc(sizeof(*lists[1]))};
    for (int i = 0; i < 2; i++) {
        Node curr = lists[i];
        curr->x = rand() % 100 - 40;
        int count = rand() % 20;  // Random length for the list
        int tmp_value;
        while (count > 0) {
            curr->next = malloc(sizeof(*lists[i]));
            // tmp_value = rand() % 1;
            tmp_value = rand() % 400;
            tmp_value = tmp_value ? (rand() % 5) : (-1);
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
    // printf("List1 before:\n\t%s\n", list1_before);

    Node merged_actual = NULL, merged_expected = NULL;
    Node *merge_actual_address = (rand() % 10) ? &merged_actual : NULL;
    ErrorCode merge_status =
        mergeSortedLists(lists[0], lists[1], merge_actual_address);
    char *list1_after = listToString(lists[0]);
    char *list2_after = listToString(lists[1]);
    if (merge_actual_address && lists[0] && lists[1] &&
        isListSorted(lists[0]) && isListSorted(lists[1])) {
        merged_expected = sortedMergeSol(lists[0], lists[1]);
    }

    bool status = true;
    if (lists[0] == NULL || lists[1] == NULL) {
        if (merge_status != EMPTY_LIST) {
            printf("Extecped status EMPTY_LIST (%d) but got %d\n", EMPTY_LIST,
                   merge_status);
            status = false;
        }
    } else if (!isListSorted(lists[0]) || !isListSorted(lists[1])) {
        if (merge_status != UNSORTED_LIST) {
            printf("Extecped status UNSORTED_LIST (%d) but got %d\n",
                   UNSORTED_LIST, merge_status);
            status = false;
        }
    } else if (merge_actual_address == NULL) {
        if (merge_status != NULL_ARGUMENT) {
            printf("Extecped status NULL_ARGUMENT (%d) but got %d\n",
                   NULL_ARGUMENT, merge_status);
            status = false;
        }
    } else if (merge_status == MEMORY_ERROR) {
        if (merged_actual != NULL) {
            printf(
                "Extecped merged_actual (MEMORY_ERROR) to be NULL but got %p\n",
                (void *)merged_actual);
            status = false;
        }
    } else {
        status = status && areListsEqual(merged_actual, merged_expected);
    }
    char *merged_expected_str = NULL, *merged_actual_str = NULL;

    if (strcmp(list1_before, list1_after) ||
        strcmp(list2_before, list2_after) || !status) {
        fprintf(stderr, "\n\n======= Test #%d Failed ========\n", test_num);

        fprintf(stderr, "List1 before:\n");
        fprintf(stderr, "\t%s\n", list1_before);
        fprintf(stderr, "List2 before:\n");
        fprintf(stderr, "\t%s\n", list2_before);

        fprintf(stderr, "List1 after:\n");
        fprintf(stderr, "\t%s\n", list1_after);
        fprintf(stderr, "List2 after:\n");
        fprintf(stderr, "\t%s\n", list2_after);

        fprintf(stderr, "Merged expected:\n");
        merged_expected_str = listToString(merged_expected);
        fprintf(stderr, "\t%s\n", merged_expected_str);
        fprintf(stderr, "Merged actual:\n");
        merged_actual_str = listToString(merged_actual);
        fprintf(stderr, "\t%s\n", merged_actual_str);
        status = false;
    }
    free(list1_before);
    free(list1_after);
    free(list2_before);
    free(list2_after);
    free(merged_expected_str);
    free(merged_actual_str);

    freeList(merged_actual);
    freeList(merged_expected);
    freeList(lists[0]);
    freeList(lists[1]);
    return status;
}

void printUsage(char *name) { printf("Usage:\n%s [NUM_OF_TESTS]\n", name); }

int main(int argc, char **argv) {
    int count = 0, failures = 0;

    if (argc == 2 && sscanf(argv[1], "%d", &count) == 1) {
        printf("Running %d random tests...\n", count);
    } else {
        printUsage(argv[0]);
        return 1;
    }
    srand(time(0));

    for (int i = 0; i < count; i++) {
        if (!testOnce(i)) {
            failures++;
        }
    }

    printf("%d failures out of %d tests\n", failures, count);
}