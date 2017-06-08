#include <GL/glew.h>
#include <cstdio>
#include <png.h>
#include "application.hpp"
#include "scene_box.hpp"
#include "util.hpp"

static Scene *currentScene = NULL;
static Scene *newScene = NULL;
static GLFWwindow *window = NULL;

int Application::bufferWidth;
int Application::bufferHeight;
constexpr float Application::elapse;
const int Application::width;
const int Application::height;

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
    glfwWindowHint(GLFW_DECORATED, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Test", NULL, NULL);
    CHECK(window) << "fail to create window";
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, keyboardCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    LOG << "init GLEW";
    glewExperimental = GL_TRUE;
    CHECK_EQ(glewInit(), GLEW_OK) << "fail to init GLEW";

    int major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    LOG << "opengl version: " << major << "." << minor;
    CHECK(major >= 3) << "not supporting gl version below 3";

    int rect[4];
    glGetIntegerv(GL_VIEWPORT, rect);
    bufferWidth = rect[2];
    bufferHeight = rect[3];

    LOG << "framebuffer: " << bufferWidth << "x" << bufferHeight;
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

void Application::screenShot(const char *name) {
    FILE *fp;
    CHECK(fp = fopen(name, "wb")) << "open " << name << " failed!";
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                  NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);
    CHECK(!setjmp(png_jmpbuf(png))) << "write " << name << " data failed!";

    png_init_io(png, fp);
    png_set_IHDR(png, info, bufferWidth, bufferHeight, 8,
                 PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(png, info);

    unsigned char *imageBuffer = new unsigned char[bufferWidth * bufferHeight * 3];

    glReadPixels(0, 0, bufferWidth, bufferHeight, GL_RGB, GL_UNSIGNED_BYTE, imageBuffer);

    for (int y = 0; y < bufferHeight; ++y) {
        int yy = bufferHeight - y - 1;
        png_write_row(png, imageBuffer + yy * bufferWidth * 3);
    }
    png_write_end(png, NULL);

    png_destroy_write_struct(&png, &info);
    fclose(fp);
}

void Application::setCursor(double x, double y) {
    glfwSetCursorPos(window, x, y);
}

void Application::getCursor(double &x, double &y) {
    glfwGetCursorPos(window, &x, &y);
}

int Application::getKey(int key) {
    return glfwGetKey(window, key);
}
