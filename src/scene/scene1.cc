#include "../ext.hpp"
#include "object_box.hpp"
#include "scene_box.hpp"

using namespace glm;

static constexpr float speed = 10.0f;
static constexpr float scrollSpeed = 5.0f;
static constexpr float mouseSpeed = 0.1f;
static constexpr double centerX = Application::width / 2;
static constexpr double centerY = Application::height / 2;

class Scene1 : public Scene {
    double timer;
    int tick;
    vec3 position, dir, left;
    float horizonAngle, verticalAngle;
    float fov;
public:
    void setup() {
        Transformer *transform = ObjectBox::danmakuTransform(this);
        objects.push_back(ObjectBox::skybox(this));
        objects.push_back(transform);
        // objects.push_back(ObjectBox::danmaku(this, transform));
        objects.push_back(ObjectBox::character(this, Model::reimu()));

        Scene::setup();

        timer = glfwGetTime();
        tick = 0;

        position = vec3(0.0f, 0.0f, 41.0f);
        horizonAngle = M_PI / 2.0f;
        verticalAngle = 0.0f;
        fov = 45.0f;
        Application::setCursor(centerX, centerY);
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

        static char str[100];
        static int second = 0;
        if (!(second % 2) && Application::getKey(GLFW_KEY_X) == GLFW_PRESS) {
        // if (!(second % 2) && second >= 180) {
            sprintf(str, "tmp/s_%04d.png", second / 2);
            LOG << str;
            Application::screenShot(str);
        }
        second++;
    }

    void update() {
        double posX, posY;
        Application::getCursor(posX, posY);
        Application::setCursor(centerX, centerY);

        horizonAngle += mouseSpeed * Application::elapse * (centerX - posX);
        verticalAngle += mouseSpeed * Application::elapse * (centerY - posY);

        dir = vec3(cos(verticalAngle) * cos(horizonAngle),
                        cos(verticalAngle) * sin(horizonAngle),
                        sin(verticalAngle));

        left = vec3(-sin(horizonAngle), cos(horizonAngle), 0.0f);

        if (Application::getKey(GLFW_KEY_W) == GLFW_PRESS){
            position += dir * Application::elapse * speed;
        }

        if (Application::getKey(GLFW_KEY_S) == GLFW_PRESS){
            position -= dir * Application::elapse * speed;
        }

        if (Application::getKey(GLFW_KEY_D) == GLFW_PRESS){
            position -= left * Application::elapse * speed;
        }

        if (Application::getKey(GLFW_KEY_A) == GLFW_PRESS){
            position += left * Application::elapse * speed;
        }

        if (Application::getKey(GLFW_KEY_E) == GLFW_PRESS) {
            fov -= scrollSpeed * Application::elapse;
        }

        if (Application::getKey(GLFW_KEY_Q) == GLFW_PRESS) {
            fov += scrollSpeed * Application::elapse;
        }

        if (Application::getKey(GLFW_KEY_Z) == GLFW_PRESS) {
            LOG << "position: " << position.x << " "
                << position.y << " " << position.z;
            LOG << "direction: " << dir.x << " " << dir.y << " " << dir.z;
            LOG << "fov: " << fov;
        }

        Scene::update();
    }

    mat4 vMat() {
        return lookAt(position, position + dir, cross(dir, left));
    }

    mat4 pMat() {
        GLfloat wh = (GLfloat) Application::width / (GLfloat) Application::height;
        return perspective(radians(fov), wh, 1.0f, 100.0f);
    }

    Light light() {
        return Light(
            vec4(0.0f, 0.0f, 0.0f, 1.0f),
            vec3(1.0f, 0.6f, 0.2f),
            vec3(0.0f, 0.0f, 0.0f),
            vec4(0.5f, 0.0f, 0.5f / 40.0f / 40.0f, 1.0f)
        );
    }
};

Scene *SceneBox::scene1() {
    return create<Scene1>();
}
