#include "map.h"

typedef struct Pair_t {
    char* key;
    char* value;
} * Pair;

typedef struct Node_t {
    Pair data;
    Node next;
} * Node;

struct Map_t {
    Node head;
    Node itter;
    int count;
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

Map mapCreate() {
    return NULL;  // TODO: Write functionality
}

void mapDestroy(Map map) {
    return;  // TODO: Write functionality
}

Map mapCopy(Map map) {
    return NULL;  // TODO: Write functionality
}

int mapGetSize(Map map) {
    return 0;  // TODO: Write functionality
}

bool mapContains(Map map, const char* key) {
    return true;  // TODO: Write functionality
}

MapResult mapPut(Map map, const char* key, const char* data) {
    return MAP_SUCCESS;  // TODO: Write functionality
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