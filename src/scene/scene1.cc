#include "scene_ext.hpp"
#include "scene_box.hpp"
#include "object_box.hpp"

using namespace glm;

class Scene1 : public SceneExt {
    Character *reimu;
public:
    Scene1() : SceneExt(true, false, 3) {}

    void setupObjects() {
        // Transformer *transform = ObjectBox::danmakuTransform(this);
        reimu = ObjectBox::character(this, Model::reimu(), Model::test2(), false);
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

    void script() {
        reimu->teleport(0.0f, 4.0f, 40.0f);
        reimu->loop(10, 40);
        // await();
        for (;;) {
            await(reimu->moveTo(0.0f, 4.0f, 45.0f, 2.0f));
            await(reimu->moveTo(4.0f, 4.0f, 45.0f, 2.0f));
            await(reimu->moveTo(4.0f, 4.0f, 40.0f, 2.0f));
            await(reimu->moveTo(0.0f, 4.0f, 40.0f, 2.0f));
        }
    }
};

Scene *SceneBox::scene1() {
    return create<Scene1>();
}
