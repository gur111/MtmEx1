#ifndef VOID_MAP_H_
#define VOID_MAP_H_

#include <stdbool.h>

#include "../mtm_map/map.h"

typedef enum {
    INT_TYPE, STR_TYPE, MAP_TYPE, UNKNOWN
} AugMapType;

/**
 * Holds the different status codes that the various function return
 */
typedef enum {
    AUG_MAP_SUCCESS,
    AUG_MAP_OUT_OF_MEMORY,
    AUG_MAP_INVALID_TYPE,
    AUG_MAP_INVALID_KEY,
    AUG_MAP_NULL_ARGUMENT,
    AUG_MAP_ITEM_ALREADY_EXISTS,
    AUG_MAP_ITEM_DOES_NOT_EXIST,
    // We only support 2 layers less mapDestroy will get way too complicated
    AUG_MAP_TOO_MANY_LAYERS,
    AUG_MAP_ERROR
} AugMapResult;

/**
 * Stands for AugmentedMap. It's a wrapper to Map.
 * It allows storing multiple types values (as specified in AugMapType).
 * Each instance needs to store values of a consistent type.
 * Keys are Integers.
 */
typedef struct AugMap_t *AugMap;

/**
 * Creates an augmented map that can store values of the given type.
 * @param type The type of values that can be stored. See AugMapType
 * @return The created augmented map. NULL on error.
 */
AugMap augMapCreate(AugMapType type);

/**
 * Get a value (of type AugMap) from the given AugMap
 * @param map The AugMap to find the value in
 * @param key The key of the requested value
 * @param result A pointer to a variable which will hold the result
 * @return The detailed result status of the function.
 */
AugMapResult augMapGetMap(AugMap map, int key, AugMap *result);

/**
 * Get a value (of type string) from the given AugMap
 * @param map The AugMap to find the value in
 * @param key The key of the requested value
 * @param result A pointer to a variable which will hold the result
 * @return The detailed result status of the function
 */
AugMapResult augMapGetStr(AugMap map, int key, char **result);

/**
  * Get a value (of type int) from the given AugMap
 * @param map The AugMap to find the value in
 * @param key The key of the requested value
 * @param result A pointer to a variable which will hold the result
 * @return The detailed result status of the function
 */
AugMapResult augMapGetInt(AugMap map, int key, int *result);

/**
 * Puts a value inside `map`
 * @param map The AugMap which will store the value
 * @param key The key to store the value under
 * @param data The data (AugMap) to be stored
 * @return The detailed result status of the function
 */
AugMapResult augMapPutMap(AugMap map, int key, AugMap data);

/**
 * Puts a value inside `map`
 * @param map The AugMap which will store the value
 * @param key The key to store the value under
 * @param data The data (string) to be stored
 * @return The detailed result status of the function
 */
AugMapResult augMapPutStr(AugMap map, int key, const char *data);

/**
 * Puts a value inside `map`
 * @param map The AugMap which will store the value
 * @param key The key to store the value under
 * @param data The data (int) to be stored
 * @return The detailed result status of the function
 */
AugMapResult augMapPutInt(AugMap map, int key, int data);

/**
 * Removes a value from the AugMap.
 * The function automagically detects the type of the value and frees
 *  it appropriately
 * @param map The AugMap the remove the value from
 * @param key The key under which the value is stored
 * @param data The data (string) to be stored
 * @return The detailed result status of the function
 */
AugMapResult augMapRemove(AugMap map, int key);

/**
 * Checks if an AugMap has a key
 * @param map The AugMap to search
 * @param key The key to look for
 * @param result A pointer to the variable which will contain the result
 * @return The detailed result status of the function
 */
AugMapResult augMapContains(AugMap map, int key, bool *result);

#ifndef NDEBUG

/**
 * Gets the type of value the given AugMap stores
 * @param map The map to check on
 * @return The AugMap type
 */
AugMapType augMapGetType(AugMap map);

#endif

// Handle foreach iterations
/**
 * Gets the key of an arbitrary element to be referred to as the first element.
 * Used for the macro AUG_MAP_FOREACH
 * @param map The AugMap to work on
 * @return The key of that "first element"
 */
int augMapGetFirst(AugMap map);

/**
 * Gets the key of another element that was never introduced since the last
 *  `augMapGetFirst` call.
 * Used for the macro AUG_MAP_FOREACH
 * @param map The AugMap to work on
 * @return The key of that "next element"
 */
int augMapGetNext(AugMap map);

/**
 * Destroys the given AugMap, freeing all it's elements if need be.
 * !! The given address will point to an unallocared section after this call.
 * @param map The AugMap to be freed
 */
void augMapDestroy(AugMap map);

/**
 * Converts the given AugMap to a simple string-to-string map.
 * After this operation. The AugMap is considered destroyed.
 * This function cannot fail (well, I can a solar flair will do the trick but
 *                              everything is relative)
 * @param map The AugMap to convert
 * @return The data from AugMap in a simple Map form
 */
Map augMapConvertToMap(AugMap map);


/**
 *
 * @param map The AugMap whitch we want to check the size
 * @return an int - the size of the AugMap->map.
 */
int augMapGetSize(AugMap map);

/*!
 * Macros for iterating over the map's values.
 * Declares a new iterator for the loop.
 */
int augMapGetMinKey(AugMap map);

#define AUG_MAP_FOREACH(iterator, map)                 \
    for (int iterator = augMapGetFirst(map); iterator != -1; \
         iterator = augMapGetNext(map))

#endif