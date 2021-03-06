#ifndef __VERTEX_DANMAKU
#define __VERTEX_DANMAKU

#include "common.hpp"

typedef struct {
    vec4 time;
    vec3 position;
    vec3 velocity;
    vec4 acceleration;
    vec4 uvIndex;
    vec3 fade;
} Vertex;

#endif
