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
#include <thread>
#include <mutex>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/random.hpp>

#include "application.hpp"
#include "renderer.hpp"
#include "scene.hpp"

inline std::ostream &operator <<(std::ostream &o, const vec2 &v) {
    o << "(" << v.x << ", " << v.y << ")";
    return o;
};

inline std::ostream &operator <<(std::ostream &o, const vec3 &v) {
    o << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return o;
};

inline std::ostream &operator <<(std::ostream &o, const vec4 &v) {
    o << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return o;
};

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

inline vec3 getMatTranslate(const glm::mat4 &mat) {
    return vec3(mat[3]);
}

inline void setMatTranslate(glm::mat4 &mat, const vec3 &v) {
    mat[3] = vec4(v, 1.0f);
}

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

#endif
