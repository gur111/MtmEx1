#include "map.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char *key;
    char *value;
} Pair;

typedef struct Node_t {
    Pair data;
    struct Node_t *next;
} *Node;

struct Map_t {
    Node head;
    // Points to the current node while iterating
    Node itter;
    int size;
};

/**
 *  mapGetNode: Returns the node associated with a specific key in the map(not a
 * copy). *			Iterator status unchanged
 * @param map - The map for which to get the node from.
 * @param key - The key of the node we want to get.
 * @return
 *  NULL if a NULL pointer was sent or if the map does not contain the requested
 *  key.
 * 	Otherwise, pointer to the node associated with the given key.
 */
static Node mapGetNode(Map map, const char *key);

static inline void nodeFree(Node node) {
    free(node->data.key);
    free(node->data.value);
    free(node);
}

/**
 * listFree: frees all of the nodes in the list until a Null point (end of the list)
 * @param node - a pointer to the head of the linked list
 * @return
 * void function, dose not return a value.
 */
static void listFree(Node node) {
    if (node == NULL) {
        return;
    }

    Node prev;
    while (node) {
        prev = node;
        node = node->next;
        nodeFree(prev);
    }
}

/**
 * nodeCreate - creates a single Node, to witch the input strings will be copied by value.
 * @param key - a string that will be inserted to the Node as a key, not Null
 * @param value - a string that will be inserted to the Node as a value, not Null
 * @return:
 *      NULL if there`s a problem with memory allocation
 *      else, the new node that was created
 */
static Node nodeCreate(const char *key, const char *value) {
    assert(key);
    assert(value);

    Node node = malloc(sizeof(*node));

    if (node == NULL) {
        return NULL;
    }

    // To avoid accessing invalid addresses (and sets size to 0)
    memset(node, 0, sizeof(*node));

    node->data.key = malloc(strlen(key) + 1);
    node->data.value = malloc(strlen(value) + 1);

    if (node->data.key == NULL || node->data.value == NULL) {
        nodeFree(node);
        return NULL;
    }

    strcpy(node->data.key, key);
    strcpy(node->data.value, value);

    return node;
}

Map mapCreate() {
    Map map = malloc(sizeof(*map));

    if (map == NULL) {
        return NULL;
    }

    memset(map, 0, sizeof(*map));

    return map;
}

void mapDestroy(Map map) {
    mapClear(map);
    free(map);
}

Map mapCopy(Map map) {
    if (map == NULL) {
        return NULL;
    }

    Map new_map = mapCreate();

    if (new_map == NULL) {
        return NULL;
    }

    MapResult status;
    //going through all of the keys in the input map and inserting them to the new map
    MAP_FOREACH(item, map) {
        status = mapPut(new_map, map->itter->data.key, map->itter->data.value);
        if (status != MAP_SUCCESS) {
#ifndef NDEBUG
            fprintf(stderr, "mapPut returned status: (%d)\n", status);
#endif
            mapDestroy(new_map);
            return NULL;
        }
    }

    new_map->size = map->size;

    return new_map;
}

int mapGetSize(Map map) {
    if (map == NULL) {
        return -1;
    }

    return map->size;
}

inline bool mapContains(Map map, const char *key) {
    return mapGet(map, key) == NULL ? false : true;
}

MapResult mapPut(Map map, const char *key, const char *value) {
    if (map == NULL || key == NULL || value == NULL) {
        return MAP_NULL_ARGUMENT;
    }

    // Try getting existing node
    Node node = mapGetNode(map, key);

    if (node == NULL) {
        // No existing node found; Create a new one
        node = nodeCreate(key, value);

        if (node == NULL) {
            return MAP_OUT_OF_MEMORY;
        }

        // We attach the new node to the list
        node->next = map->head;
        map->head = node;
        map->size++;
    } else {
        // If we found an existing node,
        // we resize the memory for value and changing the old value to the input value
        char *new_value_memory = realloc(node->data.value, strlen(value) + 1);

        if (new_value_memory == NULL) {
            return MAP_OUT_OF_MEMORY;
        }

        strcpy(new_value_memory, value);

        // We used realloc so no need to use free on the old address
        node->data.value = new_value_memory;
    }

    return MAP_SUCCESS;
}

static Node mapGetNode(Map map, const char *key) {
    if (map == NULL || key == NULL) {
        return NULL;
    }
    Node curr;
    curr = map->head;
    while (curr != NULL) {
        if (strcmp(curr->data.key, key) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

char *mapGet(Map map, const char *key) {
    //calling the function mapGetNode in order to get the Node that has the same key value as the input key
    Node key_node = mapGetNode(map, key);
    if (key_node == NULL) {
        return NULL;
    }
    return key_node->data.value;
}

MapResult mapRemove(Map map, const char *key) {
    if (map == NULL || key == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    Node curr;
    Node prev = NULL;
    curr = map->head;
    MAP_FOREACH(item, map) {
        //going through all the keys in the map, in order to find the node that saves the desired key-value
        if (strcmp(item, key) == 0) {
            if (prev == NULL) {
                map->head = curr->next;
            } else {
                prev->next = curr->next;
            }
            //removing the node from the map
            map->size--;
            nodeFree(curr);
            return MAP_SUCCESS;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
    return MAP_ITEM_DOES_NOT_EXIST;
}

char *mapGetFirst(Map map) {
    if (map == NULL || map->size == 0) {
        return NULL;
    }

    map->itter = map->head;
    return map->itter->data.key;
}

char *mapGetNext(Map map) {
    if (map == NULL || map->itter->next == NULL) {
        return NULL;
    }

    map->itter = map->itter->next;

    if (map->itter == NULL) {
        return NULL;
    }

    return map->itter->data.key;
}

MapResult mapClear(Map map) {
    if (map == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    //removing all the nodes in the map
    listFree(map->head);
    map->size = 0;
    map->head = map->itter = NULL;
    return MAP_SUCCESS;
}
