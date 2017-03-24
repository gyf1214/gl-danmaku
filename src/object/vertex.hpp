#ifndef __VERTEX
#define __VERTEX

#include <glm/glm.hpp>

typedef struct {
    glm::vec4 time;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
} Vertex;

constexpr int vertexSize = 1000;
constexpr int vertexDirections = 10;

#endif
