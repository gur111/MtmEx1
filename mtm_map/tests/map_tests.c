#include <stdio.h>
#include "../../test_utilities.h"
#include "../map.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

bool createMap_t() {
    Map map1 = mapCreate();
    Map map2 = mapCreate();
    ASSERT_TEST(map1 != NULL);
    ASSERT_TEST(map2 != NULL);
    mapPut(map1, "mary", "poppins");
    mapDestroy(map1);
    ASSERT_TEST(mapGet(map1, "mary") == NULL);
    ASSERT_TEST(mapGetSize(map1) == -1);
    ASSERT_TEST(map2 != NULL);
    mapDestroy(map2);
    return true;
}

bool put_get_func_t() {
    Map map1 = mapCreate();
    char *str1 = (char *) malloc(sizeof(char) * 6);
    strcpy(str1, "virus");
    char *str2 = (char *) malloc(7);
    strcpy(str2, "corona");
    char *str3 = (char *) malloc(7);
    strcpy(str3, "daniel");
    MapResult res = mapPut(map1, str1, str2);
    ASSERT_TEST(res == MAP_SUCCESS);
    res = mapPut(NULL, str1, str2);
    ASSERT_TEST(res == MAP_NULL_ARGUMENT);
    res = mapPut(map1, NULL, str2);
    ASSERT_TEST(res == MAP_NULL_ARGUMENT);
    res = mapPut(map1, str1, NULL);
    ASSERT_TEST(res == MAP_NULL_ARGUMENT);
    free(str2);
    char *res_srt = mapGet(map1, str1);
    ASSERT_TEST(strcmp(res_srt, "corona"));
    res_srt = mapGet(map1, "dani");
    ASSERT_TEST(res_srt == NULL);
    res = mapPut(map1, str1, str3);
    ASSERT_TEST(res == MAP_SUCCESS);
    res_srt = mapGet(map1, str1);
    ASSERT_TEST(strcmp(res_srt, str3));
    free(str1);
    free(str3);
    res_srt = mapGet(map1, "virus");
    ASSERT_TEST(strcmp(res_srt, "daniel"));
    return true;
}

int main(int argc, char *argv[]) {
    printf("Start Map Tests");
    put_get_func_t();
    createMap_t();
    return 0;
}


