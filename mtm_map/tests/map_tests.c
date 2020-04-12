//
// Created by alex bondar on 4/12/20.
//

#include <stdio.h>
//#include "../../test_utilities.h"
#include "../map.h"

int main(int argc, char *argv[]) {
    printf("Start Map Tests");
    Map map = mapCreate();
    mapPut(map,"dani","bondar");
    mapPut(map,"gur","telem");
    mapPut(map,"Virginia","Mosby");
    mapGet(map,"gur");
    mapRemove(map,"gur");
    return 0;
}

