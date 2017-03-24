#ifndef __VERTEX
#define __VERTEX

#include <glm/glm.hpp>

typedef struct {
    glm::vec4 time;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
} Vertex;

constexpr int vertexSize = 120;
constexpr int vertexDirections = 6;

const glm::vec3 vertexDir[] = {
    glm::vec3( 1.0f,  0.0f,  0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3( 0.0f,  1.0f,  0.0f),
    glm::vec3( 0.0f, -1.0f,  0.0f),
    glm::vec3( 0.0f,  0.0f,  1.0f),
    glm::vec3( 0.0f,  0.0f, -1.0f),
};

#endif
