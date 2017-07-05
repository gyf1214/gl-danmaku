#include "layer_program.hpp"

static float vertices[] = {
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f, 1.0f
};

static GLuint vbo = 0;

LayerProgram::LayerProgram(GLuint program) : program(program) {}

void LayerProgram::setup() {
    LOG << "setup layer program";

    if (!vbo) {
        LOG << "create quad buffer";
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        LOG << "buffer: " << vbo;
    } else {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    LOG << "vertex array: " << vao;

    glUseProgram(program);
    uv = glGetAttribLocation(program, "uv");
    color = glGetUniformLocation(program, "color");
    depth = glGetUniformLocation(program, "depth");
    LOG << "program: " << program;
    LOG << "uv attribute: " << uv;
    LOG << "color uniform: " << color;
    LOG << "depth uniform: " << depth;

    glUniform1i(color, 0);
    glUniform1i(depth, 1);

    glEnableVertexAttribArray(uv);
    glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
}

void LayerProgram::reset() {
    LOG << "reset layer program";
    LOG << "delete vao: " << vao;
    glDeleteVertexArrays(1, &vao);
}

void LayerProgram::bind(Layer *layer) {
    glUseProgram(program);
    glBindVertexArray(vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, layer->colorTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, layer->depthTexture());
}

void LayerProgram::render() { glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); }
