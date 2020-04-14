#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Broken version:
char *stringDuplicator(char *s, int times) {
    assert(!s);
    assert(times > 0);
    int LEN = strlen(s);
    char *out = malloc(LEN * times);
    assert(out);
    for (int i = 0; i < times; i++) {
    out = out + LEN;
    strcpy(out, s);
    }
    return out;
}


/* Everything that is wrong with it
 * Conventions Mistakes:
 * Function name isn't a verb
 * Param "s" isn't descriptive
 * "s" isn't const
 * Local "LEN" should be lower cased
 * "LEN" isn't const
 * 
 * Programmatic Errors:
 * assert(!s) is checking the opposite of what we want (it's making sure the value IS NULL)
 * malloc not allocating space for null terminator ('\0')
 * malloc isn't verified (asserts are gone when not using a debug version)
 * return is not returning the beggining of the string
 */







// Fixes version:
char *stringDuplicate(const char *src, int times) {
    assert(src);
    assert(times > 0);
    const int len = strlen(src);
    char *const out = malloc(len * times + 1);

    if (!out) {
        return NULL;
    }

    for (int i = 0; i < times; i++) {
        strcpy(out+sizeof(char)*i*len, src);
    }

    return out;
}