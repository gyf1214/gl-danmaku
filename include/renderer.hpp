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
        vec3 now, begin, target;
        float current, end;
    } transform;

    struct {
        bool moving;
        glm::quat now, begin, target;
        float current, end;
    } rotate;

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

    void teleport(vec3 pos);
    void teleport(float x, float y, float z) { teleport(vec3(x, y, z)); }
    void move(vec3 pos);
    void move(float x, float y, float z) { move(vec3(x, y, z)); }
    float moveTo(vec3 pos, float speed);
    float moveTo(float x, float y, float z, float speed) {
        return moveTo(vec3(x, y, z), speed);
    }

    void lookAt(glm::quat dir);
    float rotateTo(glm::quat dir, float speed);
    float rotateLocal(glm::quat r, float speed);
    float rotateGlobal(glm::quat r, float speed);

    virtual vec3 getGlobal(int bone, vec3 pos) = 0;
};

#endif
