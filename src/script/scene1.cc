#include "common.hpp"

using namespace glm;

class Scene1 : public BasicScript {
    Character *reimu, *suwako;
public:
    Renderer *createObjects() {
        LOG << "create objects for scene1";

        LOG << "create temp layers";
        Layer *layer1 = push(Layer::basic());
        // Layer *layer2 = push(Layer::basic());

        LOG << "create root renderer";
        root = ObjectBox::target(layer1);

        LOG << "create camera & light";
        camera = push(Camera::free(vec3(0.0f, 25.0f, 60.0f), -M_PI / 2.0f, 0.0f, 45.0f));
        light = push(LightManager::basic());
        light->ambient(vec3(0.0f, 0.0f, 0.0f));
        light->point(vec3(0.0f, 0.0f, 0.0f), vec3(0.9f, 0.4f, 0.1f), 30.0f, 0.5f);

        LOG << "create character";
        reimu = push(Character::basic(Model::reimu(), Model::test2()));
        suwako = push(Character::basic(Model::suwako(), Model::test2()));

        LOG << "create particle";
        // Particle *trail1 = push(Particle::trail(
        //     push(reimu->bindPoint(0, vec3(-9.5f, 13.8f, 0.2f)))));
        // Particle *trail2 = push(Particle::trail(
        //     push(reimu->bindPoint(0, vec3( 9.5f, 13.8f, 0.2f)))));
        Particle *danmaku = push(Particle::danmaku(Provider::danmaku1()));

        LOG << "create opaque objects";
        Renderer *opaque = push(ObjectBox::opaque(light, layer1));
        opaque->push(ObjectBox::character(reimu, camera, light));
        opaque->push(ObjectBox::character(suwako, camera, light));
        opaque->push(ObjectBox::skybox(camera, light));
        opaque->push(ObjectBox::skybox_dynamic(camera, light));

        LOG << "create transparent objects";
        Renderer *transparent = push(ObjectBox::transparent(layer1));

        // LOG << "create trail";
        // Renderer *trail = push(ObjectBox::layer());
        // push(ObjectBox::trail(trail1, camera, Layer::temp(), 0.1f, vec3(1.0f)));
        // push(ObjectBox::trail(trail2, camera, Layer::temp(), 0.1f, vec3(1.0f)));

        LOG << "create danmaku";
        transparent->push(ObjectBox::danmaku(danmaku, camera));

        LOG << "create other objects";
        push(ObjectBox::debug());

        tracks.push_back(root);
        return root;
    }

    void run() {
        reimu->teleport(0.0f, 20.0f, 59.0f);
        suwako->teleport(0.0f, -20.0f, 59.0f);
        suwako->lookAt(quat(vec3(0.0f, 0.0f, M_PI)));
        reimu->loop(10, 40);
        // suwako->loop(10, 40);
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
