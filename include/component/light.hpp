#ifndef __COM_LIGHT_MANAGER
#define __COM_LIGHT_MANAGER

#include "../core.hpp"

struct Light {
    vec4 position;
    vec3 color;
    vec3 ambient;
    vec4 material;

    Light(const vec4 &pos, const vec3 &color,
          const vec3 &ambient, const vec4 &material)
        : position(pos), color(color), ambient(ambient),
          material(material) {}
};

class LightManager : public virtual Component {
public:
    virtual const Light &light(int pass) const = 0;
    virtual Light &light(int pass) = 0;
    virtual int count(void) const = 0;
    virtual int pass(void) const = 0;
    virtual int &pass(void) = 0;

    const Light &current(void) const { return light(pass()); }

    virtual void remove(int pass) = 0;
    virtual void push(const Light &light) = 0;
    virtual void pushAmbient(vec3 color) = 0;
    virtual void pushPoint(vec3 pos, vec3 color) = 0;
    virtual void pushParallel(vec3 dir, vec3 color) = 0;
};

#endif
