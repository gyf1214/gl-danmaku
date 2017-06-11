#include "scene_ext.hpp"

using namespace glm;

static constexpr float speed = 10.0f;
static constexpr float scrollSpeed = 5.0f;
static constexpr float mouseSpeed = 0.1f;
static constexpr double centerX = Application::width / 2;
static constexpr double centerY = Application::height / 2;
static constexpr float wh = (float)Application::width / (float)Application::height;

SceneExt::SceneExt(bool debug, bool output, int passes)
    : debug(debug), output(output), passes(passes) {}

void SceneExt::setup() {
    setupObjects();

    Scene::setup();

    glEnable(GL_DEPTH_TEST);
    timer = glfwGetTime();
    tick = frame = 0;
    Application::setCursor(centerX, centerY);

    setupCamera();
}

void SceneExt::render() {
    glDisable(GL_BLEND);
    Scene::render();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    for (currentPass = 1; currentPass < passes; ++currentPass) {
        Scene::render();
    }

    ++tick;
    double now = glfwGetTime();
    if (now - timer > 1.0) {
        LOG << "fps: " << tick;
        tick = 0;
        timer = now;
    }

    static char str[20];
    if (output || (debug && Application::getKey(GLFW_KEY_X) == GLFW_PRESS)) {
        sprintf(str, "tmp/s_%04d.png", frame);
        LOG << "sceneshot: " << str;
        Application::screenShot(str);
    }

    ++frame;
}

void SceneExt::update() {
    Scene::update();
    if (!debug) return;

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
}

glm::mat4 SceneExt::vMat() {
    return lookAt(position, position + dir, cross(dir, left));
}

glm::mat4 SceneExt::pMat() {
    return perspective(radians(fov), wh, 1.0f, 100.0f);
}

Light SceneExt::light() {
    return lightPass(currentPass);
}
