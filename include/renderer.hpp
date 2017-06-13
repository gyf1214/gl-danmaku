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
        vec3 pos, begin, target;
        glm::mat4 rot;
        float current, end;
    } transform;

    void updateFrame(void);
    void updateMotion(void);
public:
    Character(Scene *scene);

    void resume(void);
    void pause(void);
    void play(float start, float speed = 1.0f);
    float playRange(float start, float end, float speed = 1.0f);
    float playTo(float end, float speed = 1.0f);
    void loop(float start, float end, float speed = 1.0f);
    void loopTo(float end, float speed = 1.0f);
    void fix(float start);

    void teleport(glm::vec3 pos);
    void teleport(float x, float y, float z) { teleport(vec3(x, y, z)); }
    void move(glm::vec3 pos);
    void move(float x, float y, float z) { move(vec3(x, y, z)); }
    float moveTo(glm::vec3 pos, float speed);
    float moveTo(float x, float y, float z, float speed) {
        return moveTo(vec3(x, y, z), speed);
    }
};

#endif
