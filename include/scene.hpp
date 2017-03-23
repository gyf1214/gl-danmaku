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
    glm::mat4 _vMat, _pMat;
public:
    virtual ~Scene() {}
    virtual void setup(void)  { for (auto o : objects) if (o) o -> setup(); }
    virtual void update(void) { for (auto o : objects) if (o) o -> update(); }
    virtual void render(void) { for (auto o : objects) if (o) o -> render(); }
    virtual void reset(void)  { for (auto o : objects) ObjectBox::release(o); }
    virtual void onKey(int key, int scancode, int action, int mods) {}
    const GLfloat *vMat(void) { return &_vMat[0][0]; }
    const GLfloat *pMat(void) { return &_pMat[0][0]; }
};

#endif
