#include "q1.h"

#include <assert.h>

/** Compares the tho nodes' values and returns the
 *  address of the node with the smaller value.
 * If one is NULL, the other node's address is returned.
 * !! WARNING: Assumes that at least one of the nodes isn't NULL
 */
Node *safeGetMinNode(Node *list1, Node *list2) {
    assert(*list1 != NULL || *list2 != NULL);

    if (*list1 == NULL) {
        return list2;
    } else if (*list2 == NULL) {
        return list1;
    } else if ((*list1)->x < (*list2)->x) {
        return list1;
    } else {
        return list2;
    }
}

void freeList(Node list) {
    if (list == NULL) {
        return;
    }
    Node prev;
    while (list) {
        prev = list;
        list = list->next;
        free(prev);
    }
}

ErrorCode mergeSortedLists(Node list1, Node list2, Node *merged_out) {
    // Arguments verifications and allocation of HEAD
    if (merged_out == NULL) {
        return NULL_ARGUMENT;
    }
    *merged_out = NULL;

    if (list1 == NULL || list2 == NULL) {
        return EMPTY_LIST;
    }

    if (!isListSorted(list1) || !isListSorted(list2)) {
        return UNSORTED_LIST;
    }


    Node head = malloc(sizeof(*head));
    if (head == NULL) {
        return MEMORY_ERROR;
    }

    // Merge the lists

    Node curr = head;
    do {
        curr->next = malloc(sizeof(*curr));
        curr = curr->next;
        if (curr == NULL) {
            // TODO: Free from had
            freeList(head);
            return MEMORY_ERROR;
        }
        Node *smaller = safeGetMinNode(&list1, &list2);
        // curr->x = 10;
        curr->x = (*smaller)->x;
        *smaller = (*smaller)->next;

    } while (list1 != NULL || list2 != NULL);
    curr->next = NULL;
    curr = head;
    head = head->next;
    free(curr);

    *merged_out = head;
    return SUCCESS;
}
