#ifndef __SCENE_EXT
#define __SCENE_EXT

#include "../ext.hpp"
#include "scene.hpp"

class SceneExt : public Scene {
protected:
    double timer;
    int tick, frame;
    vec3 position, dir, left;
    float horizonAngle, verticalAngle;
    float fov;
    bool debug, output;
    int currentPass, passes;

    static Light ambient(float r, float g, float b);
    static Light point(vec3 position, vec3 color, float radius, float alpha);
    static Light direction(vec3 dir, vec3 color);
public:
    static constexpr float Near = 1.0f;
    static constexpr float Far = 100.0f;

    SceneExt(bool debug, bool output, int passes);

    void setup(void);
    void render(void);
    void update(void);
    glm::mat4 vMat(void);
    glm::mat4 pMat(void);
    int pass(void);

    virtual void setupObjects(void) = 0;
    virtual void setupCamera(void) = 0;
};

#endif
