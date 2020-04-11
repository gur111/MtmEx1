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