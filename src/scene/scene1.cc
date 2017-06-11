#include "scene_ext.hpp"
#include "scene_box.hpp"
#include "object_box.hpp"

using namespace glm;

class Scene1 : public SceneExt {
public:
    Scene1() : SceneExt(true, false, 1) {}

    void setupObjects() {
        Transformer *transform = ObjectBox::danmakuTransform(this);
        objects.push_back(ObjectBox::skybox(this));
        objects.push_back(ObjectBox::character(this, Model::reimu()));
        objects.push_back(transform);
        objects.push_back(ObjectBox::danmaku(this, transform));
    }

    void setupCamera() {
        position = vec3(0.0f, 0.0f, 41.0f);
        horizonAngle = M_PI / 2.0f;
        verticalAngle = 0.0f;
        fov = 45.0f;
    }

    Light lightPass(int pass) {
        return Light(
            vec4(0.0f, 0.0f, 0.0f, 1.0f),
            vec3(1.0f, 0.6f, 0.2f),
            vec3(0.0f, 0.0f, 0.0f),
            vec4(0.5f, 0.0f, 0.5f / 40.0f / 40.0f, 1.0f),
            pass
        );
    }
};

Scene *SceneBox::scene1() {
    return create<Scene1>();
}
