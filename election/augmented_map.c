
#include "void_map.h"


bool mapContains(AugMap map, int key){
    char *strKey = intToStr(key);
    bool result = mapContains(map, strKey);
    free(strKey);
    return result;
}