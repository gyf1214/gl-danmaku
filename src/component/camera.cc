#include "../ext.hpp"
#include "component/camera.hpp"

using namespace glm;

static constexpr float speed = 50.0f;
static constexpr float scrollSpeed = 5.0f;
static constexpr float mouseSpeed = 0.1f;
static constexpr double centerX = Application::width / 2;
static constexpr double centerY = Application::height / 2;
static constexpr float wh = (float)Application::width / (float)Application::height;

class CameraFree : public virtual Camera {
    vec3 pos, dir, left;
    float horizonAngle, verticalAngle;
    float fov;
public:
    CameraFree(vec3 pos, float horizon, float vertical, float fov)
        : pos(pos), horizonAngle(horizon), verticalAngle(vertical),
          fov(fov) {}

    void setup() {
        LOG << "setup free camera";
        Application::setCursor(centerX, centerY);
    }
    void reset() { LOG << "reset free camera"; }
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
            pos += dir * Application::elapse * speed;
        }

        if (Application::getKey(GLFW_KEY_S) == GLFW_PRESS){
            pos -= dir * Application::elapse * speed;
        }

        if (Application::getKey(GLFW_KEY_D) == GLFW_PRESS){
            pos -= left * Application::elapse * speed;
        }

        if (Application::getKey(GLFW_KEY_A) == GLFW_PRESS){
            pos += left * Application::elapse * speed;
        }

        if (Application::getKey(GLFW_KEY_E) == GLFW_PRESS) {
            fov -= scrollSpeed * Application::elapse;
        }

        if (Application::getKey(GLFW_KEY_Q) == GLFW_PRESS) {
            fov += scrollSpeed * Application::elapse;
        }

        if (Application::getKey(GLFW_KEY_Z) == GLFW_PRESS) {
            LOG << "pos: " << pos.x << " "
                << pos.y << " " << pos.z;
            LOG << "direction: " << dir.x << " " << dir.y << " " << dir.z;
            LOG << "fov: " << fov;
        }
    }

    vec3 position() const { return pos; }
    vec3 direction() const { return dir; }
    float fovy() const { return radians(fov); }
    mat4 vMat() const { return lookAt(pos, pos + dir, cross(dir, left)); }
    mat4 pMat() const { return perspective(radians(fov), wh, 1.0f, 300.0f); }
};

Camera *Camera::free(vec3 pos, float horizon, float vertical, float fov) {
    return Box::create<CameraFree>(pos, horizon, vertical, fov);
}
