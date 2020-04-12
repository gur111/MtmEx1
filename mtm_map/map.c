#include "map.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char* key;
    char* value;
} Pair;

typedef struct Node_t {
    Pair data;
    struct Node_t* next;
} * Node;

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
static Node mapGetNode(Map map, const char* key);

static inline void nodeFree(Node node) {
    free(node->data.key);
    free(node->data.value);
    free(node);
}

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

static Node nodeCreate(const char* key, const char* value) {
    assert(key);
    assert(value);

    Node node = malloc(sizeof(*node));

    if (node == NULL) {
        return NULL;
    }

    // To avoid accessing invalid addresses (and sets size to 0)
    memset(node, 0, sizeof(*node));

    node->data.key = malloc(sizeof(strlen(key) + 1));
    node->data.value = malloc(sizeof(strlen(value) + 1));

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
    if (map == NULL) {
        return;
    }

    listFree(map->head);
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

    MapResult status = MAP_SUCCESS;

    MAP_FOREACH(item, map) {
        status = mapPut(new_map, map->itter->data.key, map->itter->data.value);
        if (status != MAP_SUCCESS) {
#ifndef NDEBUG
            fprintf(stderr, "mapPut returned status: (%d)\n", status);
#endif
            mapDestroy(new_map);
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

inline bool mapContains(Map map, const char* key) {
    return mapGet(map, key) == NULL ? false : true;
}

MapResult mapPut(Map map, const char* key, const char* value) {
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
        // If we found an existing node, we resize the memory for value
        char* new_value_memory = realloc(node->data.value, strlen(value) + 1);

        if (new_value_memory == NULL) {
            return MAP_OUT_OF_MEMORY;
        }

        strcpy(new_value_memory, value);

        // We used realloc so no need to use free on the old address
        node->data.value = new_value_memory;
    }

    return MAP_SUCCESS;
}

static Node mapGetNode(Map map, const char* key) {
    return NULL;  // TODO: Write functionality
}
// TODO: Check if the use of the free func is correct
char* mapGet(Map map, const char* key) {
    if (map == NULL || key == NULL) {
        return NULL;
    } else {
        Node curr;
        curr = map->head;
        while (curr != NULL) {
            if (strcmp(curr->data.key, key) == 0) {
                return curr->data.value;
            }

            curr = curr->next;
        }
        return NULL;
    }
    // TODO: Try to run func
}

MapResult mapRemove(Map map, const char* key) {
    if (map == NULL || key == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    Node curr;
    Node prev;
    curr = map->head;
    prev = NULL;
    MAP_FOREACH(item, map) {
        if (strcmp(item, key) == 0) {
            prev->next = curr->next;
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

char* mapGetFirst(Map map) {
    if (map == NULL || map->size == 0) {
        return NULL;
    }

    map->itter = map->head;
    return map->itter->data.key;
}

char* mapGetNext(Map map) {
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

    listFree(map->head);
    map->size = 0;
    map->head = map->itter = NULL;
    return MAP_SUCCESS;  // TODO: try to run func
}
