#ifndef VOID_MAP_H_
#define VOID_MAP_H_
#include <stdbool.h>

typedef enum { INT_TYPE, STR_TYPE, MAP_TYPE, UNKNOWN } AugMapType;

typedef enum {
    AUG_MAP_SUCCESS,
    AUG_MAP_OUT_OF_MEMORY,
    AUG_MAP_INVALID_TYPE,
    AUG_MAP_NULL_ARGUMENT,
    AUG_MAP_ITEM_ALREADY_EXISTS,
    AUG_MAP_ITEM_DOES_NOT_EXIST,
    // We only support 2 layers less mapDestroy will get way too complicated
    AUG_MAP_TOO_MANY_LAYERS,
    AUG_MAP_ERROR
} AugMapResult;

typedef struct AugMap_t *AugMap;

AugMap augMapCreate(AugMapType type);

AugMapResult augMapGetMap(AugMap map, int key, AugMap *result);
AugMapResult augMapGetStr(AugMap map, int key, char **result);
AugMapResult augMapGetInt(AugMap map, int key, int *result);

AugMapResult augMapPutMap(AugMap map, int key, AugMap data);
AugMapResult mapPutStr(AugMap map, int key, char *data);
AugMapResult mapPutInt(AugMap map, int key, int value);

AugMapResult augMapRemove(AugMap map, int key);

AugMapResult augMapContains(AugMap map, int key, bool *result);

AugMap augMapGetFirstMap(AugMap map);

AugMap augMapGetNextMap(AugMap map);

void augMapDestroy(AugMap map);

AugMapResult augMapRemove(AugMap map, int key);

/*!
 * Macros for iterating over a map's values.
 * Declares a new iterator for the loop.
 */
#define AUG_MAP_FOREACH_MAP(iterator, map)                        \
    for (AugMap iterator = (AugMap)mapGetFirstMap(map); iterator; \
         iterator = mapGetNextMap(map))

#endif
