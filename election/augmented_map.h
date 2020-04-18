#ifndef VOID_MAP_H_
#define VOID_MAP_H_

#include "map.h"

typedef enum MapType_t { INT, STR, MAP } MapType;

typedef struct AugMap_t {
    Map map;
    MapType type;
} * AugMap;

AugMap mapCreate(MapType type);

AugMap mapGetMap(AugMap map, int key);
char *mapGetStr(AugMap map, int key);
MapResult mapPut(AugMap map, int key, AugMap data);
MapResult mapPut(AugMap map, int key, char *data);

bool mapContains(AugMap map, int key);
int mapGetSize(AugMap map);

char *mapGetFirst(AugMap map);
char *mapGetNext(AugMap map);


void mapDestroy(AugMap map);
MapResult mapRemove(Map map, int key);

#endif