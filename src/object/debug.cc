#include "ext.hpp"

class DebugTool : public virtual Object {
    double timer;
    int frame, tick;
    char str[20];
    bool output;
public:
    DebugTool(bool output) : output(output) {}

    void setup() {
        LOG << "setup debug tool";
        timer = glfwGetTime();
        tick = frame = 0;
    }

    void render() {
        ++tick;
        double now = glfwGetTime();
        if (now - timer > 1.0) {
            INFO << "fps: " << tick;
            tick = 0;
            timer = now;
        }

        if (output || Application::getKey(GLFW_KEY_X) == GLFW_PRESS) {
            sprintf(str, "tmp/s_%04d.png", frame);
            INFO << "render: " << str;
            Application::screenShot(str);
        }

        ++frame;
    }

    void reset() { LOG << "reset debug tool"; }
};

Object *ObjectBox::debug(bool output) {
    return create<DebugTool>(output);
}
