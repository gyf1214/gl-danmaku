#ifndef __APPLICATION
#define __APPLICATION

#include <GLFW/glfw3.h>

class Scene;

class Application {
    static void errorCallBack(int error, const char* desc);
    static void keyboardCallback(GLFWwindow* window, int key,
                                 int scancode, int action, int mods);
public:
    static const int width = 1024;
    static const int height = 768;
    static int bufferWidth, bufferHeight;
    static void setup(void);
    static void mainLoop(void);
    static void run(void);
    static void setScene(Scene *);
    static void setClose(void);
    static void screenShot(const char *name);
};

#endif
