#ifndef __OBJECT_BOX
#define __OBJECT_BOX

#include "core.hpp"
#include "component.hpp"

class ObjectBox : public Box {
public:
    static Scene *scene(Script *script);
    static Renderer *renderer(void);
    static Object *character(Character *character, Camera *camera, LightManager *light);
    static Object *danmaku(Particle *particle, Camera *camera);
    static Object *skybox(Camera *camera, LightManager *light);
    static Object *trail(Particle *particle, Camera *camera, float size, vec3 color);
};

#endif
