#ifndef __VERTEX_TRAIL
#define __VERTEX_TRAIL

#include "common.hpp"

typedef struct {
    vec3 position;
    float alpha;
} Vertex;

constexpr int trailSize = 100;

#endif
