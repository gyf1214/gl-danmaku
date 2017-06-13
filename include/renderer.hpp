#ifndef __RENDERER
#define __RENDERER

#include "common.hpp"

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

class Character : public Renderer {
protected:
    struct {
        bool play, loop, range;
        float current, begin, end, speed;
    } frame;

    struct {
        bool reset, moving;
        glm::mat4 mat;
        glm::vec3 begin, target;
        float current, end;
    } transform;

    void updateFrame(void);
    void updateMotion(void);
public:
    Character(Scene *scene);

    void resume(void);
    void pause(void);
    void play(float start, float speed = 1.0f);
    void playRange(float start, float end, float speed = 1.0f);
    void loop(float start, float end, float speed = 1.0f);
    void fix(float start);

    void teleport(glm::vec3 pos);
    void teleport(float x, float y, float z) { teleport(vec3(x, y, z)); }
    float moveTo(glm::vec3 pos, float speed);
    float moveTo(float x, float y, float z, float speed) {
        return moveTo(vec3(x, y, z), speed);
    }
};

#endif
