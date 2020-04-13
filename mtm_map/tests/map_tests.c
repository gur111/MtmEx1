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
    ASSERT_TEST(mapGetSize(map1) == 0);
    ASSERT_TEST(mapGetFirst(map1) == NULL);
    mapPut(map1, "mary", "poppins");
    mapDestroy(map1);
    ASSERT_TEST(mapGet(map1, "mary") == NULL);
    ASSERT_TEST(mapGetSize(map1) == -1);
    ASSERT_TEST(map2 != NULL);
    mapDestroy(map2);
    return true;
}

bool putGetFunc_t() {
    Map map1 = mapCreate();
    char *str1 = (char *) malloc(7);
    printf("pleas enter a 6 char word: \n");
    scanf("%s",str1);
    char *str1_copy = (char *) malloc(7);
    strcpy(str1_copy,str1);
    char *str2 = (char *) malloc(7);
    printf("pleas enter a 6 char word: \n");
    scanf("%s",str2);
    char *str2_copy = (char *) malloc(7);
    strcpy(str2_copy,str2);
    char *str3 = (char *) malloc(7);
    printf("pleas enter a 6 char word: \n");
    scanf("%s",str3);
    char *str3_copy = (char *) malloc(7);
    strcpy(str3_copy,str3);

    MapResult res = mapPut(NULL, str1, str2);
    ASSERT_TEST(res == MAP_NULL_ARGUMENT);
    res = mapPut(map1, NULL, str2);
    ASSERT_TEST(res == MAP_NULL_ARGUMENT);
    res = mapPut(map1, str1, NULL);
    ASSERT_TEST(res == MAP_NULL_ARGUMENT);
    //enter a key and a value, and then delete the origin and compare whit the dict
    res = mapPut(map1, str1, str2);
    ASSERT_TEST(res == MAP_SUCCESS);
    free(str2);
    char *res_srt = mapGet(map1, str1);
    ASSERT_TEST(strcmp(res_srt, str2_copy));
    //enter a not existing key
    res_srt = mapGet(map1, "dani");
    ASSERT_TEST(res_srt == NULL);
    //changing the value of an existing key
    res = mapPut(map1, str1, str3);
    ASSERT_TEST(res == MAP_SUCCESS);
    res_srt = mapGet(map1, str1);
    ASSERT_TEST(strcmp(res_srt, str3));
    free(str1);
    free(str3);
    res_srt = mapGet(map1, str1_copy);
    ASSERT_TEST(strcmp(res_srt, str3_copy));

    free(str1_copy);
    free(str2_copy);
    free(str3_copy);
    mapDestroy(map1);
    return true;
}

bool copyMap_t() {
    ASSERT_TEST(mapCopy(NULL) == NULL);
    Map map_1 = mapCreate();
    ASSERT_TEST(mapPut(map_1, "key1", "value1") == MAP_SUCCESS);
    ASSERT_TEST(mapPut(map_1, "key2", "value2") == MAP_SUCCESS);
    Map map_2 = mapCopy(map_1);
    ASSERT_TEST(strcmp(mapGet(map_2, "key1"), "value1"));
    ASSERT_TEST(strcmp(mapGet(map_2, "key2"), "value2"));
    ASSERT_TEST(mapGetSize(map_2) == mapGetSize(map_1));
    mapDestroy(map_1);
    ASSERT_TEST(strcmp(mapGet(map_2, "key1"), "value1"));
    ASSERT_TEST(strcmp(mapGet(map_2, "key2"), "value2"));
    mapDestroy(map_2);
    return true;
}

bool getSize() {
    Map map_1 = mapCreate();
    ASSERT_TEST(mapGetSize(map_1)== 0);
    ASSERT_TEST(mapPut(map_1, "key1", "value1") == MAP_SUCCESS);
    ASSERT_TEST(mapGetSize(map_1)==1);
    ASSERT_TEST(mapRemove(map_1,"key1"));
    ASSERT_TEST(mapGetSize(map_1)== 0);
    ASSERT_TEST(mapGetSize(NULL)==-1);
    mapDestroy(map_1);
    return true;
}

int main(int argc, char *argv[]) {
    printf("Start Map Tests");
    putGetFunc_t();
    copyMap_t();
    getSize();
    createMap_t();
    return 0;
}


