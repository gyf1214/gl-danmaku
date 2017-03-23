#ifndef __VERTEX
#define __VERTEX

#include <glm/glm.hpp>

typedef struct {
    glm::vec4 time;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
} Vertex;

constexpr int vertexSize = 100;

#endif
