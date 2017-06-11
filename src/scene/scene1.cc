#include "scene_ext.hpp"
#include "scene_box.hpp"
#include "object_box.hpp"

using namespace glm;

class Scene1 : public SceneExt {
public:
    Scene1() : SceneExt(true, false, 3) {}

    void setupObjects() {
        // Transformer *transform = ObjectBox::danmakuTransform(this);
        Character *reimu = (Character *)ObjectBox::character(this, Model::reimu(), Model::test2());
        reimu->play(0.0f);
        objects.push_back(ObjectBox::skybox(this));
        objects.push_back(reimu);
        // objects.push_back(transform);
        // objects.push_back(ObjectBox::danmaku(this, transform));
    }

    void setupCamera() {
        position = vec3(0.0f, 0.0f, 41.0f);
        horizonAngle = M_PI / 2.0f;
        verticalAngle = 0.0f;
        fov = 45.0f;
    }

    Light light() {
        static Light lights[] = {
            ambient(0.0f, 0.0f, 0.0f),
            direction(vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.6f, 0.2f)),
            point(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.6f, 0.2f), 40.0f, 0.5f)
        };

        CHECK(currentPass < sizeof(lights) / sizeof(Light)) << "invalid pass!";

        return lights[currentPass];
    }
};

Scene *SceneBox::scene1() {
    return create<Scene1>();
}
