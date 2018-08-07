#ifndef LENGTH_STRINGS_H
#define LENGTH_STRINGS_H

#define LS_SPREAD(ls) (ls Length), (ls Chars)

typedef struct {
    unsigned int AllocationSize;
    unsigned int Length;
    char Chars[];
} length_string;

length_string *lsMakeString(int NumBytes) {
    size_t AllocationSize = sizeof(length_string) + NumBytes;
    length_string *ls = malloc(AllocationSize);
    ls->AllocationSize = AllocationSize;
    ls->Length = NumBytes;
    return ls;
}

void *lsAssignFromCharArray(length_string *ls, char Chars[], unsigned int Length) {
    memcpy(ls->Chars, Chars, Length);
    ls->Length = Length;
}

void lsAppendChar(length_string *ls, char Character) {
    ls->Chars[ls->Length] = Character;
    ls->Length += 1;
}

#endif //LENGTH_STRINGS_H