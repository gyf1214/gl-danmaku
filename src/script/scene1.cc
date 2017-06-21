#include "common.hpp"

using namespace glm;

class Scene1 : public BasicScript {
    Character *reimu, *suwako;
public:
    void createObjects(Renderer *root) {
        LOG << "create objects for scene1";
        BasicScript::createObjects(root);

        LOG << "create camera & light";
        camera = push(Camera::free(vec3(0.0f, -10.0f, 45.0f), M_PI / 2.0f, 0.0f, 45.0f));
        light = push(LightManager::basic());
        light->ambient(vec3(0.0f, 0.0f, 0.0f));
        light->point(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.6f, 0.2f), 40.0f, 0.5f);

        LOG << "create character";
        reimu = push(Character::basic(Model::reimu(), Model::test2()));
        suwako = push(Character::basic(Model::suwako(), Model::test2()));

        LOG << "create other objects";
        push(ObjectBox::character(reimu, camera, light));
        push(ObjectBox::character(suwako, camera, light));
        push(ObjectBox::skybox(camera, light));
        push(ObjectBox::debug());
    }

    void run() {
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

Script *Script::scene1() {
    return Box::global<Scene1>();
}
