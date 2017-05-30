#ifndef __VERTEX_BASE
#define __VERTEX_BASE

#include "common.hpp"

typedef struct {
    vec3 position;
    vec3 normal;
    vec3 tangent;
    vec2 uv;
} Vertex;

#endif
