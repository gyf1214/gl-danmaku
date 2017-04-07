#ifndef __VERTEX_BASE
#define __VERTEX_BASE

#include <glm/glm.hpp>

typedef struct {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec2 uv;
} Vertex;

#endif
