#ifndef HELPERS_H
#define HELPERS_H 1

#define Min(a,b) ((a)<(b)?(a):(b))
#define Max(a,b) ((a)>(b)?(a):(b))
#define Clamp(v, a, b) (Min(Max((v), (a)), (b)))

#define FourChars(s) (uint32_t)((s[3]<<24) | (s[2]<<16) | (s[1]<<8) | s[0])

#define ArraySize(A) (sizeof(A)/sizeof(*A))

/* Program Specific*/

#endif
