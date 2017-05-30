#ifndef __COMMON
#define __COMMON

#include <cstdlib>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <png.h>

#define Offset(type, member)    ((void *)(&(((type *)0)->member)))
#define Alias(src, tar)         inline decltype(src) &tar() { return tar; }

using glm::vec2;
using glm::vec3;
using glm::vec4;

class Scene;
class Renderer;
class Transformer;

inline float randomNorm() {
    return rand() / (float)RAND_MAX;
}

inline float randomNeg() {
    return (rand() - RAND_MAX / 2) / (float)RAND_MAX;
}

inline glm::vec3 randomNorm3() {
    return normalize(glm::vec3(randomNeg(), randomNeg(), randomNeg()));
}

#endif
