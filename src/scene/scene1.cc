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
        objects.push_back(ObjectBox::skybox(this));
        objects.push_back(transform);
        objects.push_back(ObjectBox::danmaku(this, transform));

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
        // if (!(second % 1)) {
        //     sprintf(str, "tmp/s_%04d.png", second / 1);
        //     LOG << str;
        //     Application::screenShot(str);
        // }
        // second++;
    }

    glm::mat4 vMat() {
        return glm::lookAt(
            glm::vec3(1.0f, -7.0f, 2.0f),
            glm::vec3(0.0f, 0.0f, 2.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );
        // return glm::lookAt(
        //     glm::vec3(0.0f, 0.0f, 20.0f),
        //     glm::vec3(0.0f, 0.0f, 0.0f),
        //     glm::vec3(0.0f, 1.0f, 0.0f)
        // );
    }

    glm::mat4 pMat() {
        GLfloat wh = (GLfloat) Application::width / (GLfloat) Application::height;
        return glm::perspective(glm::radians(90.0f), wh, 1.0f, 100.0f);
    }
};

Scene *SceneBox::scene1() {
    return create<Scene1>();
}
