#ifndef __VERTEX_TRAIL
#define __VERTEX_TRAIL

#include "common.hpp"

typedef struct {
    vec3 position;
    float alpha;
} Vertex;

static constexpr int trailSize = 10000;
static constexpr int trailHead = 5;

#endif
