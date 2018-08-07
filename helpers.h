#ifndef HELPERS_H
#define HELPERS_H 1

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define clamp(v, a, b) (min(max((v), (a)), (b)))

#define four_chars(s) (uint32_t)((s[3]<<24) | (s[2]<<16) | (s[1]<<8) | s[0])

#endif
