#ifndef __SCENE_EXT
#define __SCENE_EXT

#include "../ext.hpp"
#include "scene.hpp"
#include "fiber.hpp"

class SceneExt : public Scene {
protected:
    double timer;
    int tick, frame;
    vec3 position, dir, left;
    float horizonAngle, verticalAngle;
    float fov;
    bool debug, output;
    int currentPass, passes;

    float nextEvent;
    bool waiting;
    Fiber *fiber;

    static Light ambient(float r, float g, float b);
    static Light point(vec3 position, vec3 color, float radius, float alpha);
    static Light direction(vec3 dir, vec3 color);

    static void fiberWorker(void *ptr);
public:
    static constexpr float Near = 1.0f;
    static constexpr float Far = 100.0f;

    SceneExt(bool debug, bool output, int passes);

    void setup(void);
    void render(void);
    void update(void);
    void reset(void);
    float fovy(void);
    glm::mat4 vMat(void);
    glm::mat4 pMat(void);
    int pass(void);

    void await(void);
    void await(float x);
    void awaitUntil(float x);

    virtual void setupObjects(void) = 0;
    virtual void setupCamera(void) = 0;
    virtual void script(void) = 0;
};

#endif
