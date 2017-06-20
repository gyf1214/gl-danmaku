#ifndef __CORE_SCENE
#define __CORE_SCENE

#include "object.hpp"

class Scene {
protected:
    std::vector<Component *> components;
    std::vector<Object *> objects;

public:
    virtual ~Scene();
    virtual void setup(void);
    virtual void update(void);
    virtual void render(void);
    virtual void reset(void);
};

#endif
