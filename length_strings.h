#ifndef LENGTH_STRINGS_H
#define LENGTH_STRINGS_H

#include <assert.h>
#include <memory.h>
#include <stdbool.h>

#define LS_SPREAD(ls) (ls Length), (ls Chars)

typedef struct length_string {
    bool Dynamic;
    size_t Length;
    char *Chars;
} length_string;

length_string lsMakeString(size_t Length, const char *Chars) {
    return (length_string){
        .dynamic = false,
        .length = Length,
        .chars = (char *)Chars,
    };
}

void lsAssignFromCharArray(length_string *ls, char Chars[], size_t Length) {
    memcpy(ls->Chars, Chars, Length);
    ls->Length = Length;
}

void lsAppendChar(length_string *ls, char Character) {
    ls->Chars[ls->Length] = Character;
    ls->Length += 1;
}

#endif //LENGTH_STRINGS_H