#include "scene.hpp"
#include "scene_box.hpp"
#include "application.hpp"
#include "util.hpp"
#include <glm/gtx/transform.hpp>
#include <cmath>
#include <cstdio>

class Scene1 : public Scene {
    double timer;
    int tick;
public:
    void setup() {
        Transformer *transform = ObjectBox::danmakuTransform(this);
        objects.push_back(transform);
        objects.push_back(ObjectBox::danmaku(this, transform));

        _vMat = glm::lookAt(
            glm::vec3(8.0f, 8.0f, 8.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );

        GLfloat wh = (GLfloat) Application::width / (GLfloat) Application::height;
        _pMat = glm::perspective(glm::radians(45.0f), wh, 1.0f, 100.0f);

        Scene::setup();

        timer = glfwGetTime();
        tick = 0;
    }

    void render() {
        Scene::render();

        ++tick;
        double now = glfwGetTime();
        if (now - timer >= 1.0) {
            timer = now;
            LOG << "fps: " << tick;
            tick = 0;
        }

        // static char str[100];
        // static int second = 0;
        // sprintf(str, "tmp/s_%04d.png", second++);
        // LOG << str;
        //
        // Application::screenShot(str);
    }
};

Scene *SceneBox::scene1() {
    return create<Scene1>();
}
