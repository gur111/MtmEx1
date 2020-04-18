
#include "void_map.h"



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


bool mapContains(AugMap map, int key){
    char *strKey = intToStr(key);
    bool result = mapContains(map, strKey);
    free(strKey);
    return result;
}