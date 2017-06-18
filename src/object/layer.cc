#include "layer.hpp"

static GLuint vao, vbo, program = 0;

Layer::Layer() {
    LOG << "generate framebuffer for layer";

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &depth);
    glBindTexture(GL_TEXTURE_2D, depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 Application::bufferWidth, Application::bufferHeight,
                 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

    glGenTextures(1, &color);
    glBindTexture(GL_TEXTURE_2D, color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 Application::bufferWidth, Application::bufferHeight,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

    LOG << "fbo: " << framebuffer;
    LOG << "color texture: " << color;
    LOG << "depth texture: " << depth;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (!program) Layer::setup();
}

void Layer::clear() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void Layer::select() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void Layer::detach() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static float vertices[] = {
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f, 1.0f
};

void Layer::setup() {
    LOG << "setup layer renderer";

    program = Shader::layer();
    glUseProgram(program);
    GLuint color = glGetUniformLocation(program, "color");
    GLuint depth = glGetUniformLocation(program, "depth");
    GLuint uv = glGetAttribLocation(program, "uv");

    LOG << "program: " << program;
    LOG << "color uniform: " << color;
    LOG << "depth uniform: " << depth;
    LOG << "uv attribute: " << uv;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    LOG << "vertex array: " << vao;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    LOG << "buffer: " << vbo;

    glEnableVertexAttribArray(uv);
    glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);

    glUniform1i(color, 0);
    glUniform1i(depth, 1);
}

void Layer::render() {
    glUseProgram(program);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    glDepthMask(GL_FALSE);

    glBindVertexArray(vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, color);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depth);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}

Layer *Layer::temp() {
    static Layer *ret = NULL;
    if (!ret) ret = new Layer();
    return ret;
}
