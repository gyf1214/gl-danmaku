#ifndef __RENDERER
#define __RENDERER

class Scene;

#include <GL/glew.h>

class Renderer {
protected:
    Scene *scene;
public:
    Renderer(Scene *scene);
    virtual ~Renderer();
    virtual void setup(void);
    virtual void update(void);
    virtual void render(void) = 0;
    virtual void reset(void);
};

class Transformer : public Renderer {
public:
    Transformer(Scene *scene);
    virtual GLuint outputBuffer() = 0;
    void render();
};

#endif
