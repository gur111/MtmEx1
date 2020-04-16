#include <assert.h>
#include <stdio.h>
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
 * 1. Function name isn't a verb
 * 2. Param "s" isn't descriptive
 * 3. Local "LEN" should be lower cased
 * 4. "LEN" isn't const
 * 5. Indentation is wrong in the ’for’ loop
 *
 * Code Errors:
 * 1. "s" isn't const (this is a code error because it will fail to compile if you
 *                     pass a 'const char *' to the function. It will claim "passing argument 1 of
 *                     ‘stringDuplicate’ discards ‘const’ qualifier from pointer target type
 *                     [-Wdiscarded-qualifiers]", assuming of course that -Wall and -Werror were
 *                     used in compilation) 
 * 2. assert(!s) is checking the opposite of what we want (it's making sure the value IS NULL)
 * 3. malloc not allocating space for null terminator ('\0')
 * 4. malloc isn't verified (asserts are gone when not using a debug version)
 * 5. return is not returning the beggining of the string
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
        strcpy(out + sizeof(char) * i * len, src);
    }

    return out;
}

int main() {
    const char *hello = "Hello";

    printf("Multiple times hello:\n");
    for (int i = 1; i < 10; i++) {
        printf("%d times \"Hello\" is \"%s\"\n", i, stringDuplicate(hello, i));
    }

    printf("Zero length testing:\n");
    for (int i = 1; i < 5; i++) {
        printf("%d times \"\" is \"%s\"\n", i, stringDuplicate("", i));
    }
}
