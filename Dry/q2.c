#include <assert.h>
#include <stdlib.h>
#include <string.h>

char *stringDuplicator(const char *src, int times) {
    assert(src);
    assert(times > 0);
    int len = strlen(src);
    char *out = malloc(len * times + 1);

    if (!out) {
        return NULL;
    }

    for (int i = 0; i < times; i++) {
        strcat(out, src);
    }

    return out;
}