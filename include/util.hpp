#ifndef __UTIL
#define __UTIL

#include <cstdlib>
#include "log.hpp"

#define Offset(type, member)    ((void *)(&(((type *)0)->member)))
#define Alias(src, tar)         inline decltype(src) &tar() { return tar; }

inline float randomNorm() {
    return rand() / (float)RAND_MAX;
}

inline float randomNeg() {
    return (rand() - RAND_MAX / 2) / (float)RAND_MAX;
}

#endif
