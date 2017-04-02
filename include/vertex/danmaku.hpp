#ifndef __VERTEX_DANMAKU
#define __VERTEX_DANMAKU

#include <glm/glm.hpp>

typedef struct {
    glm::vec4 time;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec4 uvIndex;
} Vertex;

constexpr int vertexSize = 1440;

#endif
