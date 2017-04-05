#ifndef __APPLICATION
#define __APPLICATION

#include <GLFW/glfw3.h>

class Scene;

class Application {
    static void errorCallBack(int error, const char* desc);
    static void keyboardCallback(GLFWwindow* window, int key,
                                 int scancode, int action, int mods);
public:
    static constexpr int width = 1024;
    static constexpr int height = 768;
    static constexpr float elapse = 1.0f / 60.0f;
    static int bufferWidth, bufferHeight;
    static void setup(void);
    static void mainLoop(void);
    static void run(void);
    static void setScene(Scene *);
    static void setClose(void);
    static void screenShot(const char *name);
    static void setCursor(double x, double y);
    static void getCursor(double &x, double &y);
    static int getKey(int key);
};

#endif
