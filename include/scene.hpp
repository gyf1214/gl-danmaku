#ifndef __SCENE
#define __SCENE

#include "light.hpp"

class Renderer;

class Scene {
protected:
    std::vector<Renderer *> objects;
public:
    virtual ~Scene();
    virtual void setup(void);
    virtual void update(void);
    virtual void render(void);
    virtual void reset(void);
    virtual void onKey(int key, int scancode, int action, int mods);
    virtual glm::mat4 vMat(void) = 0;
    virtual glm::mat4 pMat(void) = 0;
    virtual Light light(void) = 0;
    virtual int pass(void) = 0;
};

#endif
