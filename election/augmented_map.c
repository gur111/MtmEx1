#include "augmented_map.h"

#include <assert.h>

#include "../mtm_map/map.h"

// Supports upto 64 bit. On 99% of systems it will be 11 inc \0
#define MAX_PTR_AS_STR_SIZE 20
// Supports up to 64 bit integers
#define MAX_INT_AS_STR_SIZE 22

struct AugMap_t {
    Map map;
    AugMapType type;
};

static char *intToStr(int num) {
    char *str = malloc(MAX_INT_AS_STR_SIZE * sizeof(char));

    if (str == NULL) {
        return NULL;
    }

    sprintf(str, "%d", num);

    char *shrunken_str = realloc(str, strlen(str) + 1);
    if (shrunken_str == NULL) {
        free(str);
        return NULL;
    }

    return shrunken_str;
}

static int strToInt(char *str) {
    assert(str != NULL);

    int num;
    sscanf(str, "%d", &num);

    return num;
}

static char *ptrToStr(void *ptr) {
    // It's practically impossible to have a pointer which isn't of size 4
    //  which means that the str would be of size 11 (including null
    //  terminator). Still we allow up to size 20 characters to make you happy.
    char *ptr_as_str = malloc(sizeof(char) * MAX_PTR_AS_STR_SIZE);

    if (ptr_as_str == NULL) {
        return NULL;
    }

    sprintf(ptr_as_str, "%p", ptr);
    // Shrinking the allocation to fix the string exactly
    char *final_str = realloc(ptr_as_str, strlen(ptr_as_str) + 1);
    if (final_str == NULL) {
        free(ptr_as_str);
        return NULL;
    }

    return final_str;
}

static void *strToPtr(char *ptr_as_str) {
    void *ptr = NULL;
    assert(ptr_as_str != NULL);
    sscanf(ptr_as_str, "%p", &ptr);
    assert(ptr != NULL);

    return ptr;
}

AugMap augMapCreate(AugMapType type) {
    AugMap map = malloc(sizeof(*map));

    if (map == NULL) {
        return NULL;
    }

    memset(map, 0, sizeof(*map));

    if ((map->map = mapCreate()) == NULL) {
        free(map);
        return NULL;
    }
    map->type = type;

    return map;
}

#ifndef NDEBUG
AugMapType augMapGetType(AugMap map) {
    return map == NULL ? UNKNOWN : map->type;
}
#endif

/**
 * Gets a string value from map (not a copy).
 * Enforces expected type after conversion.
 * TODO: Complete docstring
 */
static AugMapResult augMapGet(AugMap map, AugMapType type, int key,
                              char **result) {
    if (map == NULL || result == NULL) {
        return AUG_MAP_NULL_ARGUMENT;
    }

    // Assignment it internal; Assert is enough
    assert(map->map);

    if (type != UNKNOWN && map->type != type) {
        return AUG_MAP_INVALID_TYPE;
    }

    char *str_key = intToStr(key);

    if (str_key == NULL) {
        return AUG_MAP_OUT_OF_MEMORY;
    }

    char *str_value = mapGet(map->map, str_key);
    free(str_key);

    if (str_value == NULL) {
        return AUG_MAP_ITEM_DOES_NOT_EXIST;
    }

    *result = str_value;

    return AUG_MAP_SUCCESS;
}

AugMapResult augMapGetMap(AugMap map, int key, AugMap *result) {
    if (result == NULL) {
        return AUG_MAP_NULL_ARGUMENT;
    }
    *result = NULL;

    char *str_value;
    AugMapResult status = augMapGet(map, MAP_TYPE, key, &str_value);
    if (status != AUG_MAP_SUCCESS) {
        return status;
    }

    assert(str_value != NULL);

    *result = (AugMap)strToPtr(str_value);
    return AUG_MAP_SUCCESS;
}

/**
 * Gets a copy of the string value previously stored in the given key.
 * Return value is the status of the operation.
 * Result is store in `result`
 */
AugMapResult augMapGetStr(AugMap map, int key, char **result) {
    if (result == NULL) {
        return AUG_MAP_NULL_ARGUMENT;
    }
    *result = NULL;

    char *str_value;
    AugMapResult status = augMapGet(map, STR_TYPE, key, &str_value);
    if (status != AUG_MAP_SUCCESS) {
        return status;
    }

    assert(str_value != NULL);
    *result = malloc(strlen(str_value) + 1);
    if (*result == NULL) {
        return AUG_MAP_OUT_OF_MEMORY;
    }

    strcpy(*result, str_value);

    return AUG_MAP_SUCCESS;
}

AugMapResult augMapGetInt(AugMap map, int key, int *result) {
    if (result == NULL) {
        return AUG_MAP_NULL_ARGUMENT;
    }
    *result = 0;

    char *str_value;
    AugMapResult status = augMapGet(map, INT_TYPE, key, &str_value);
    if (status != AUG_MAP_SUCCESS) {
        return status;
    }

    assert(str_value != NULL);

    *result = strToInt(str_value);

    return AUG_MAP_SUCCESS;
}

AugMapResult mapResultToAugMapResult(MapResult result) {
    // Remap error codes
    switch (result) {
        case MAP_SUCCESS:
            return AUG_MAP_SUCCESS;

        case MAP_OUT_OF_MEMORY:
            return AUG_MAP_OUT_OF_MEMORY;

        case MAP_NULL_ARGUMENT:
            return AUG_MAP_NULL_ARGUMENT;

        case MAP_ITEM_ALREADY_EXISTS:
            return AUG_MAP_ITEM_ALREADY_EXISTS;

        case MAP_ITEM_DOES_NOT_EXIST:
            return AUG_MAP_ITEM_DOES_NOT_EXIST;

        default:
            return AUG_MAP_ERROR;
    }
}

static AugMapResult augMapPut(AugMap map, AugMapType type, int key,
                              char *str_value) {
    if (map == NULL || str_value == NULL) {
        return AUG_MAP_NULL_ARGUMENT;
    }

    assert(map->map);

    if (map->type != type) {
        return AUG_MAP_INVALID_TYPE;
    }

    char *str_key = intToStr(key);

    if (str_key == NULL) {
        return AUG_MAP_OUT_OF_MEMORY;
    }

    AugMapResult status =
        mapResultToAugMapResult(mapPut(map->map, str_key, str_value));
    free(str_key);

    return status;
}

AugMapResult augMapPutMap(AugMap map, int key, AugMap data) {
    if (data == NULL) {
        return AUG_MAP_NULL_ARGUMENT;
    }
    // Make sure we only make 2 layers of map
    if (data->type == MAP_TYPE) {
        return AUG_MAP_TOO_MANY_LAYERS;
    }

    char *str_value = ptrToStr(data);
    if (str_value == NULL) {
        return AUG_MAP_OUT_OF_MEMORY;
    }

    AugMapResult status = augMapPut(map, MAP_TYPE, key, str_value);
    free(str_value);
    return status;
}

AugMapResult augMapPutStr(AugMap map, int key, char *data) {
    return augMapPut(map, STR_TYPE, key, data);
}

AugMapResult augMapPutInt(AugMap map, int key, int value) {
    char *str_value = intToStr(value);
    if (str_value == NULL) {
        return AUG_MAP_OUT_OF_MEMORY;
    }

    AugMapResult status = augMapPut(map, INT_TYPE, key, str_value);
    free(str_value);
    return status;
}

AugMapResult augMapContains(AugMap map, int key, bool *result) {
    if (result == NULL) {
        return AUG_MAP_NULL_ARGUMENT;
    }
    *result = false;

    char *str_key = intToStr(key);
    if (str_key == NULL) {
        return AUG_MAP_OUT_OF_MEMORY;
    }

    char *value_str;
    AugMapResult status = augMapGet(map, UNKNOWN, str_key, &value_str);
    free(str_key);
    // Should never happen when passing UNKNOWN
    assert(status != AUG_MAP_INVALID_TYPE);

    switch (status) {
        case AUG_MAP_SUCCESS:
            *result = true;
        case AUG_MAP_ITEM_DOES_NOT_EXIST:
            return AUG_MAP_SUCCESS;
        case AUG_MAP_OUT_OF_MEMORY:
        case AUG_MAP_NULL_ARGUMENT:
        default:
            return status;
    }
}

AugMapResult augMapRemove(AugMap map, int key) {
    if (map == NULL) {
        return AUG_MAP_NULL_ARGUMENT;
    }
    char *str_key = intToStr(key);
    if (str_key == NULL) {
        return AUG_MAP_OUT_OF_MEMORY;
    }
    AugMapResult status;
    // If the value is of type AugMap, get it and destroy it (avoid mem leaks)
    if (map->type == MAP_TYPE) {
        AugMap sub_map;
        status = augMapGetMap(map, key, &sub_map);
        if (status != AUG_MAP_SUCCESS) {
            free(str_key);
            return status;
        }

        augMapDestroy(sub_map);
    }
    status = mapResultToAugMapResult(mapRemove(map->map, str_key));
    free(str_key);
    return status;
}

AugMap augMapGetFirstMap(AugMap map) {
    if (map == NULL || map->type != MAP_TYPE) {
        return NULL;
    }

    return (AugMap)strToPtr(mapGetFirst(map->map));
}

AugMap augMapGetNextMap(AugMap map) {
    if (map == NULL || map->type != MAP_TYPE) {
        return NULL;
    }
    return (AugMap)strToPtr(mapGetNext(map->map));
}

/**
 * "Recursively" free map with all of its submaps (if there are any)
 */
void augMapDestroy(AugMap map) {
    if (map == NULL) {
        return;
    }
    // Shouldn't happen but idc
    assert(map->type != UNKNOWN);

    if (map->type == MAP_TYPE) {
        // It may seem recursive, it may smell recursive, it may sound, it may
        // even taste recursive It isn't really, sub maps will never be map of
        // maps. See `augMapPutMap`
        AUG_MAP_FOREACH_MAP(sub_map, map) { augMapDestroy(sub_map); }
    }

    mapDestroy(map->map);
    free(map);
}