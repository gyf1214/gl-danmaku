#include <GL/glew.h>
#include "application.hpp"
#include "scene_box.hpp"
#include "util.hpp"

static Scene *currentScene = NULL;
static Scene *newScene = NULL;
static GLFWwindow *window = NULL;

void Application::errorCallBack(int error, const char* description) {
    FAIL << "glfw error " << error << ": " << description;
}

void Application::keyboardCallback(GLFWwindow* window, int key,
                                   int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        setClose();
    }
    currentScene -> onKey(key, scancode, action, mods);
}

void Application::setup() {
    LOG << "init GLFW";
    CHECK(glfwInit()) << "fail to init GLFW";
    glfwSetErrorCallback(errorCallBack);

    LOG << "create window";
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Test", NULL, NULL);
    CHECK(window) << "fail to create window";
    glfwMakeContextCurrent(window);
    // glfwSwapInterval(0);
    glfwSetKeyCallback(window, keyboardCallback);

    LOG << "init GLEW";
    glewExperimental = GL_TRUE;
    CHECK_EQ(glewInit(), GLEW_OK) << "fail to init GLEW";

    int major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    LOG << "opengl version: " << major << "." << minor;
    CHECK(major >= 3) << "not supporting gl version below 3";

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::setScene(Scene *scene) {
    newScene = scene;
}

void Application::setClose() {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Application::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        
        if (newScene != currentScene) {
            LOG << "change scene";
            SceneBox::release(currentScene);
            currentScene = newScene;
            if (currentScene) currentScene -> setup();
        }

        if (currentScene) {
            currentScene -> update();
            currentScene -> render();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    if (currentScene) SceneBox::release(currentScene);
}

void Application::run() {
    setup();
    mainLoop();
}
