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
public:
    static constexpr float Near = 1.0f;
    static constexpr float Far = 100.0f;

    SceneExt(bool debug, bool output, int passes);

    void setup(void);
    void render(void);
    void update(void);
    glm::mat4 vMat(void);
    glm::mat4 pMat(void);
    Light light(void);

    virtual void setupObjects(void) = 0;
    virtual void setupCamera(void) = 0;
    virtual Light lightPass(int pass) = 0;
};

#endif
