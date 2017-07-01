#include "../ext.hpp"
#include "component/layer.hpp"
#include "component/shader.hpp"
#include "component/texture.hpp"

static GLuint vao, vbo, program = 0;

static float vertices[] = {
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f, 1.0f
};

static void setupRenderer() {
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

class LayerImp : public virtual Layer {
    GLuint framebuffer, depth, color;
public:
    void setup() {
        LOG << "generate framebuffer for layer";

        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        depth = Texture::genDepth();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

        color = Texture::genScreen();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

        LOG << "fbo: " << framebuffer;
        LOG << "color texture: " << color;
        LOG << "depth texture: " << depth;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        if (!program) setupRenderer();
    }

    void reset() {
        LOG << "reset layer";
        LOG << "delete framebuffer: " << framebuffer;
        glDeleteFramebuffers(1, &framebuffer);
        LOG << "delete depth texture: " << depth;
        glDeleteTextures(1, &depth);
        LOG << "delete color texture: " << color;
        glDeleteTextures(1, &color);
    }

    void select() { glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); }

    void attach() {
        glUseProgram(program);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
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

        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
    }

    GLuint colorTexture() { return color; }
    GLuint depthTexture() { return depth; }
};

void Layer::release() {
    if (program) {
        LOG << "reset layer renderer";
        LOG << "delete array: " << vao;
        glDeleteVertexArrays(1, &vao);
        LOG << "delete buffer: " << vbo;
        glDeleteBuffers(1, &vbo);
    }
}

void Layer::clear(float alpha, const vec3 &color) {
    glClearColor(color.x, color.y, color.z, alpha);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void Layer::detach() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Layer *Layer::basic() {
    return Box::create<LayerImp>();
}

Layer *Layer::temp() {
    static Layer *ret = NULL;
    if (!ret) {
        ret = Box::global<LayerImp>();
        ret->setup();
    }
    return ret;
}
