#include "common.hpp"

using namespace glm;

static constexpr int trailCount = 4;

class Scene1 : public BasicScript {
    Character *reimu, *suwako;
    Motion *center, *lookat;
    Renderer *transparent;
    Particle *trails[trailCount];
    Motion *trailBinds[trailCount];
public:
    Renderer *createObjects() {
        LOG << "create objects for scene1";

        LOG << "create temp layers";
        Layer *layer1 = push(Layer::basic());
        // Layer *layer2 = push(Layer::basic());

        LOG << "create root renderer";
        root = ObjectBox::target(layer1);
        // root = ObjectBox::renderer();

        LOG << "create camera & light";
        center = push(Motion::spline());
        lookat = push(Motion::spline());
        // camera = push(Camera::free(vec3(0.0f, 25.0f, 60.0f), -M_PI / 2.0f, 0.0f, 45.0f));
        camera = push(Camera::twoNode(center, lookat, 45.0f));
        light = push(LightManager::basic());
        light->ambient(vec3(0.0f, 0.0f, 0.0f));
        light->point(vec3(0.0f, 0.0f, 0.0f), vec3(0.9f, 0.4f, 0.1f), 30.0f, 0.5f);

        LOG << "create character";
        reimu = push(Character::basic(Model::reimu(), Model::test2()));
        suwako = push(Character::basic(Model::suwako(), Model::test2()));

        LOG << "create particle";
        Particle *trail1 = push(Particle::trail(
            push(reimu->bindPoint(-1, vec3(-6.5f, 13.8f, 0.3f)))));
        Particle *trail2 = push(Particle::trail(
            push(reimu->bindPoint(-1, vec3( 6.5f, 13.8f, 0.3f)))));

        for (int i = 0; i < trailCount; ++i) {
            trailBinds[i] = push(Motion::spline());
            trails[i] = push(Particle::trail(trailBinds[i]));
            trails[i]->enable(false);
        }

        Particle *danmaku = push(Particle::danmaku(Provider::danmaku1()));

        LOG << "create opaque objects";
        Renderer *opaque = push(ObjectBox::opaque(light, layer1));
        opaque->push(ObjectBox::character(reimu, camera, light));
        opaque->push(ObjectBox::character(suwako, camera, light));
        opaque->push(ObjectBox::skybox(camera, light));
        opaque->push(ObjectBox::skybox_dynamic(camera, light));

        LOG << "create transparent objects";
        transparent = push(ObjectBox::transparent(layer1));

        LOG << "create trail";
        // Renderer *trail = push(ObjectBox::layer());
        transparent->push(ObjectBox::trail(trail1, camera, 0.1f, vec3(1.0f)));
        transparent->push(ObjectBox::trail(trail2, camera, 0.1f, vec3(1.0f)));

        for (int i = 0; i < trailCount; ++i) {
            transparent->push(ObjectBox::trail(trails[i], camera,
                                               0.5f, vec3(0.5f, 0.5f, 1.0f)));
        }

        LOG << "create danmaku";
        transparent->push(ObjectBox::danmaku(danmaku, camera));

        // push(ObjectBox::trail(trail1, camera, 0.1f, vec3(1.0f)));

        LOG << "create other objects";
        push(ObjectBox::debug());

        tracks.push_back(root);
        return root;
    }

    void run() {
        // reimu back
        center->teleport(0.5f, 22.0f, 61.0f);
        lookat->teleport(0.0f, 0.0f, 60.0f);

        reimu->teleport(0.0f, 20.0f, 59.0f);
        suwako->teleport(0.0f, -20.0f, 59.0f);
        suwako->lookAt(quat(vec3(0.0f, 0.0f, M_PI)));
        reimu->loop(10, 40);

        // suwako close
        await(2.0f);
        center->teleport(0.2f, -18.5f, 60.5f);
        lookat->teleport(0.0f, -20.0f, 60.6f);

        // reimu close
        await(1.5f);
        center->teleport(1.0f, 18.6f, 60.7f);
        lookat->teleport(0.0f, 20.0f, 60.8f);

        // suwako close
        await(1.5f);
        center->teleport(0.2f, -18.5f, 60.5f);
        lookat->teleport(0.0f, -20.0f, 60.6f);

        // move to reimu back
        await(0.5f);
        lookat->moveTo(0.0f, 0.0f, 60.0f, 0.5f);
        center->waypoint(0.1f, 17.0f, 60.1f, 0.5f);
        center->waypoint(3.0f, 30.0f, 62.0f, 1.0f);

        // 1-non light
        await(0.5f);
        light->point(vec3(0.0f, -20.0f, 60.0f), vec3(1.0f, 1.0f, 0.8f), 15.0f, 0.5f, 1.0f);

        // reimu close
        await(1.5f);
        center->teleport(0.7f, 18.5f, 60.7f);
        lookat->teleport(0.0f, 20.0f, 60.6f);

        // reimu above
        await(0.7f);
        center->teleport(7.0f, 30.0f, 65.0f);
        lookat->teleport(0.0f, 16.0f, 60.0f);

        // move to reimu side
        awaitUntil(9.0f);
        reimu->waypoint(2.0f, 20.0f, 60.1f, 0.8f);

        // from reimu above to side over view
        await(0.5f);
        center->waypoint(9.0f, 20.0f, 62.1f, 1.0f);
        lookat->waypoint(0.0f, 19.0f, 61.0f, 1.0f);
        center->waypoint(40.0f, 0.0f, 62.5f, 1.5f);
        await(lookat->waypoint(0.0f, 0.0f, 62.0f, 1.5f));

        // 1-non end
        light->remove(2, 0.5f);

        // suwako side
        await(1.3f);
        center->teleport(15.0f, -10.0f, 59.5f);
        lookat->teleport(0.0f, -20.0f, 60.0f);

        // suwako close
        await(1.2f);
        center->teleport(0.2f, -18.5f, 60.4f);
        lookat->teleport(0.0f, -20.0f, 60.5f);

        // suwako far
        await(2.0f);
        center->teleport(0.1f, 17.0f, 61.0f);
        lookat->teleport(0.0f, 0.0f, 60.0f);

        awaitUntil(17.0f);
        light->point(vec3(0.0f, -10.0f, 60.0f), vec3(1.0f, 0.5f, 0.5f), 20.0f, 0.5f, 2.0f);

        // move to remimu back
        await(0.5f);
        await(center->moveTo(0.1f, 30.0f, 66.0f, 1.0f));

        // reimu close
        await(0.5f);
        center->moveTo(3.5f, 17.6f, 61.7f, 0.5f);
        lookat->moveTo(2.0f, 20.0f, 61.8f, 0.5f);

        // reimu side
        awaitUntil(21.0f);
        center->teleport(9.6f, 21.0f, 60.1f);
        lookat->teleport(0.0f, 19.5f, 60.0f);

        // spin camera with reimu
        await(0.5f);
        lookat->moveTo(0.0f, 10.0f, 58.5f, 1.5f);
        reimu->waypoint(-3.0f, 15.0f, 58.0f, 0.75f);
        await(reimu->waypoint(-3.0f, 10.0f, 58.5f, 0.75f));

        // reimu close
        await(0.5f);
        center->teleport(-2.5f, 6.0f, 59.7f);
        lookat->teleport(-3.0f, 10.0f, 59.5f);

        await(0.5f);
        trails[0]->enable(true);
        trails[1]->enable(true);
        trailBinds[0]->teleport(-3.4f, 10.0f, 59.0f);
        trailBinds[0]->waypoint(-3.4f, 10.0f, 61.0f, 0.5f);
        trailBinds[0]->waypoint(-3.8f,  5.0f, 63.0f, 0.5f);
        trailBinds[0]->waypoint(-2.0f, -10.0f, 60.0f, 0.5f);
        trailBinds[0]->waypoint(-0.0f, -20.0f, 60.0f, 0.5f);
        trailBinds[0]->waypoint( 6.0f, -50.0f, 60.0f, 1.0f);
        trailBinds[1]->teleport(-2.6f, 10.0f, 59.0f);
        trailBinds[1]->waypoint(-2.6f, 10.0f, 61.0f, 0.5f);
        trailBinds[1]->waypoint(0.0f, 4.0f, 60.5f, 0.5f);
        trailBinds[1]->waypoint(1.0f, -9.0f, 60.5f, 0.5f);
        trailBinds[1]->waypoint(0.0f, -20.0f, 60.5f, 0.5f);
        trailBinds[1]->waypoint(-4.6f, -50.0f, 60.5f, 1.0f);

        // move to danmaku
        await(0.5f);
        center->waypoint(3.0f, 15.0f, 59.8f, 0.5f);
        lookat->waypoint(-6.0f, 5.0f, 59.2f, 0.5f);
        center->waypoint(-3.0f, 20.0f, 60.1f, 0.5f);
        await(lookat->waypoint(-1.5f, 0.0f, 59.0f, 0.5f));

        // suwako
        await(0.5f);
        suwako->waypoint(4.0f, -20.0f, 59.0f, 0.8f);
        center->teleport(0.7f, -13.0f, 62.0f);
        lookat->teleport(3.0f, -20.0f, 59.2f);

        // await(0.5f);
        // reimu->waypoint(-3.0f, 5.0f, 58.5f, 0.7f);
        // reimu->waypoint(-5.0f, 5.0f, 60.0f, 0.7f);

        // suwako close
        await(1.0f);
        trails[0]->enable(false);
        trails[1]->enable(false);
        light->remove(2, 1.0f);
        lookat->teleport(4.0f, -20.0f, 60.0f);
        center->teleport(0.0f, -21.0f, 60.5f);

        // follow suwako
        lookat->waypoint(6.0f, -18.0f, 60.0f, 1.0f);
        lookat->waypoint(15.0f, -15.0f, 58.0f, 1.0f);
        center->waypoint(2.0f, -22.0f, 61.5f, 1.0f);
        center->waypoint(10.0f, -23.0f, 62.0f, 1.0f);
        suwako->waypoint(6.0f, -18.0f, 59.0f, 1.0f);
        await(suwako->waypoint(15.0f, -15.0f, 57.0f, 1.0f));

        // follow suwako
        lookat->waypoint(18.0f, -10.0f, 58.0f, 1.0f);
        lookat->waypoint(20.0f, -5.0f, 58.0f, 1.0f);
        lookat->waypoint(20.0f, 5.0f, 58.0f, 1.0f);
        lookat->waypoint(20.0f, 15.0f, 58.0f, 1.0f);
        center->waypoint(15.0f, -35.0f, 65.0f, 1.0f);
        center->waypoint(18.0f, -30.0f, 65.0f, 1.0f);
        center->waypoint(18.0f, -20.0f, 65.0f, 1.0f);
        center->waypoint(18.0f, -10.0f, 65.0f, 1.0f);

        suwako->waypoint(18.0f, -10.0f, 57.5f, 1.0f);
        suwako->waypoint(20.0f, -5.0f, 57.5f, 1.0f);
        suwako->waypoint(22.0f,  5.0f, 58.0f, 1.0f);
        await(suwako->waypoint(20.0f,  15.0f, 58.0f, 1.0f));

        await(0.5f);
        center->teleport(15.0f, 20.0f, 62.0f);
        await(0.5f);
        suwako->moveTo(20.0f, 15.0f, 75.0f, 2.0f);

        // reimu close
        await(1.0f);
        lookat->teleport(-3.0f, 10.0f, 60.0f);
        center->teleport(4.1f, 11.8f, 65.0f);

        // switch to suwako;
        await(1.0f);
        center->moveTo(4.1f, 11.8f, 70.0f, 1.0f);
        lookat->waypoint(5.0f, 2.0f, 67.0f, 0.5f);
        await(lookat->waypoint(20.0f, 15.0f, 75.0f, 1.0f));

        await(1.0f);
        lookat->moveTo(6.0f, 12.5f, 67.0f, 2.0f);
        await(center->moveTo(-30.0f, -2.5f, 58.0f, 2.0f));

        await(1.0f);
        reimu->waypoint(-13.0f, 7.5f, 62.0f, 1.0f);
        reimu->waypoint(-15.0f, 6.7f, 67.0f, 1.0f);

        // follow reimu
        await(1.0f);
        lookat->teleport(-13.0f, 7.5f, 62.5f);
        center->teleport(-16.0f, -5.0f, 65.5f);
        lookat->waypoint(-15.0f, 6.7f, 67.5f, 1.0f);
        center->waypoint(-25.0f, 7.0f, 68.5f, 1.0f);

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
