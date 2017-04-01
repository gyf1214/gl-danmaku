#ifndef __SCENE
#define __SCENE

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "renderer.hpp"
#include "object_box.hpp"

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
};

#endif
