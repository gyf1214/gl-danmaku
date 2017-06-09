#ifndef __EXT
#define __EXT

#include "common.hpp"

#include <cstdio>
#include <cstring>
#include <cmath>
#include <ctime>
#include <string>
#include <iostream>
#include <algorithm>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/random.hpp>

#include "application.hpp"
#include "renderer.hpp"
#include "scene.hpp"
#include "log.hpp"

inline float randomNorm() {
    return rand() / (float)RAND_MAX;
}

inline float randomNeg() {
    return (rand() - RAND_MAX / 2) / (float)RAND_MAX;
}

inline vec3 randomNorm3() {
    return normalize(vec3(randomNeg(), randomNeg(), randomNeg()));
}

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

#endif
