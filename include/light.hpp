#ifndef __LIGHT
#define __LIGHT

#include "common.hpp"

struct Light {
    vec4 position;
    vec3 color;
    vec3 ambient;
    vec4 material;
    int pass;

    Light(const vec4 &pos, const vec3 &color,
          const vec3 &ambient, const vec4 &material, int pass)
        : position(pos), color(color), ambient(ambient),
          material(material), pass(pass) {}
};

#endif
