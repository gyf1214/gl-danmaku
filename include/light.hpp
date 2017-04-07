#ifndef __LIGHT
#define __LIGHT

#include <glm/glm.hpp>

struct Light {
    glm::vec4 position;
    glm::vec3 color;
    glm::vec3 ambient;
    glm::vec4 material;

    Light(const glm::vec4 &pos, const glm::vec3 &color,
          const glm::vec3 &ambient, const glm::vec4 &material)
        : position(pos), color(color), ambient(ambient), material(material) {}
};

#endif
