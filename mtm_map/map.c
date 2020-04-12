#include "map.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define VERIFY_ALLOC(addr) \
    if ((addr) == NULL) {  \
        return NULL;       \
    }

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
    // Points to the current node while itterating
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
    VERIFY_ALLOC(node);

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
    VERIFY_ALLOC(node);
    // To avoid accessing invalid addresses
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

    VERIFY_ALLOC(map);
    memset(map, 0, sizeof(*map));

    return map;
}

void mapDestroy(Map map) {
    VERIFY_ALLOC(map);

    listFree(map->head);
    free(map);
    return;
}

Map mapCopy(Map map) {
    VERIFY_ALLOC(map);
    Map new_map = mapCreate();
    VERIFY_ALLOC(new_map);

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
    if (map == NULL) {
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

char* mapGet(Map map, const char* key) {
    return NULL;  // TODO: Write functionality
}

MapResult mapRemove(Map map, const char* key) {
    return MAP_SUCCESS;  // TODO: Write functionality
}

char* mapGetFirst(Map map) {
    return NULL;  // TODO: Write functionality
}

char* mapGetNext(Map map) {
    return NULL;  // TODO: Write functionality
}

MapResult mapClear(Map map) {
    return MAP_SUCCESS;  // TODO: Write functionality
}