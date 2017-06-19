#include "scene_ext.hpp"
#include "scene_box.hpp"
#include "object_box.hpp"

using namespace glm;

class Scene1 : public SceneExt {
    Character *reimu, *utsuho, *suwako;
public:
    Scene1() : SceneExt(true, false, 3) {}

    void setupObjects() {
        // Transformer *transform = ObjectBox::danmakuTransform(this);
        reimu = ObjectBox::character(this, Model::reimu(), Model::test2(), false);
        // utsuho = ObjectBox::character(this, Model::utsuho(), Model::test2(), false);
        suwako = ObjectBox::character(this, Model::suwako(), Model::test2(), false);
        objects.push_back(ObjectBox::skybox(this));
        objects.push_back(reimu);
        objects.push_back(suwako);
        // objects.push_back(transform);
        // objects.push_back(ObjectBox::danmaku(this, transform));
        Transformer *trail1 = ObjectBox::trailTransform(this, reimu, -1, vec3(-9.5f, 13.8f, 0.2f));
        Transformer *trail2 = ObjectBox::trailTransform(this, reimu, -1, vec3( 9.5f, 13.8f, 0.2f));
        objects.push_back(trail1);
        objects.push_back(trail2);
        objects.push_back(ObjectBox::trail(this, trail1, 0.15f, vec3(1.0f, 0.0f, 0.0f)));
        objects.push_back(ObjectBox::trail(this, trail2, 0.15f, vec3(1.0f, 0.0f, 0.0f)));
    }

    void setupCamera() {
        position = vec3(0.0f, -10.0f, 45.0f);
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
        reimu->teleport(-2.0f, 0.0f, 40.0f);
        suwako->teleport(2.0f, 0.0f, 40.0f);
        // reimu->loop(10, 40);
        // await(reimu->rotateTo(degreeAxis(90.0f, 0.0f, 0.0f, 1.0f), 1.0f));
        // await(reimu->playTo(60));
        // await(reimu->moveTo(0.0f, 4.0f, 45.0f, 3.0f));
        // reimu->fix(80);
        // reimu->rotateLocal(degreeAxis(30.0f, 1.0f, 0.0f, 0.0f), 1.0f);
        // await(reimu->moveTo(4.0f, 4.0f, 45.0f, 3.0f));
        // reimu->rotateLocal(degreeAxis(-30.0f, 1.0f, 0.0f, 0.0f), 1.0f);
        // await(reimu->playTo(90));
        // reimu->loop(10, 40);
    }
};

Scene *SceneBox::scene1() {
    return create<Scene1>();
}
