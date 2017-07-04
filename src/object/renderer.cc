#include "ext.hpp"

using namespace std;

static float vertices[] = {
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f, 1.0f
};

static GLuint vbo = 0;

class LayerProgram : public virtual Object {
public:
    GLuint program, vao, uv, color, depth;

    LayerProgram(GLuint program) : program(program) {}

    void setup() {
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

    void reset() {
        LOG << "reset layer program";
        LOG << "delete vao: " << vao;
        glDeleteVertexArrays(1, &vao);
    }

    void bind(Layer *layer) {
        glUseProgram(program);
        glBindVertexArray(vao);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, layer->colorTexture());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, layer->depthTexture());
    }

    void render() { glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); }
};

class BasicRenderer : public virtual Renderer {
protected:
    vector<Object *> objects;
public:
    void setup() { for (const auto &o : objects) o->setup(); }
    void reset() { for (const auto &o : objects) Box::release(o); objects.clear(); }
    void render() { for (const auto &o : objects) o->render(); }
    void push(Object *o) { objects.push_back(o); }
};

class LayerRenderer : public BasicRenderer {
protected:
    Layer *layer;
public:
    LayerRenderer(Layer *layer) : layer(layer) {}
};

class OpaqueRenderer : public LayerRenderer {
    LightManager *light;
public:
    OpaqueRenderer(LightManager *light, Layer *layer)
        : LayerRenderer(layer), light(light) {}

    void render() {
        layer->select();
        Layer::clear(1.0f);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        light->pass() = 0;
        LayerRenderer::render();

        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        glBlendEquation(GL_FUNC_ADD);

        int n = light->count();
        for (int i = 1 ; i < n; ++i) {
            light->pass() = i;
            LayerRenderer::render();
        }

        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
        glDisable(GL_DEPTH_TEST);

        // layer->snapshot();
        Layer::detach();
        // layer->blit();
    }
};

class TransparentRenderer : public LayerRenderer {
    Layer *layer0, *layer1;
    int pass;
    LayerProgram *program;
public:
    TransparentRenderer(Layer *layer, int pass)
        : LayerRenderer(layer), pass(pass) {}

    void setup() {
        layer0 = Layer::basic();
        layer0->setup();
        layer1 = Layer::basic();
        layer1->setup();

        program = Box::create<LayerProgram>(Shader::layer());
        program->setup();

        LayerRenderer::setup();
    }

    void render() {
        layer0->select();
        glClearDepth(0.0);
        Layer::clear(1.0f);
        glClearDepth(1.0);

        glEnable(GL_DEPTH_TEST);

        for (int i = 0; i < pass; ++i) {
            layer1->select();
            Layer::clear(0.0f);
            glDepthFunc(GL_LEQUAL);

            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, layer0->colorTexture());
            glActiveTexture(GL_TEXTURE6);
            glBindTexture(GL_TEXTURE_2D, layer0->depthTexture());
            glActiveTexture(GL_TEXTURE7);
            glBindTexture(GL_TEXTURE_2D, layer->depthTexture());

            LayerRenderer::render();

            layer0->select();
            program->bind(layer1);

            glDepthFunc(GL_ALWAYS);
            glEnable(GL_BLEND);
            glBlendFuncSeparate(GL_DST_ALPHA, GL_ONE, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);

            program->render();

            glDisable(GL_BLEND);
        }

        glDisable(GL_DEPTH_TEST);

        layer->select();
        program->bind(layer0);

        glEnable(GL_BLEND);
        glBlendFuncSeparate(GL_ONE, GL_SRC_ALPHA, GL_ZERO, GL_ONE);
        glBlendEquation(GL_FUNC_ADD);

        program->render();

        glDisable(GL_BLEND);

        Layer::detach();
    }

    void reset() {
        Box::release(layer0);
        Box::release(layer1);
        Box::release(program);

        LayerRenderer::reset();
    }
};

class TargetRenderer : public LayerRenderer {
    LayerProgram *program;
public:
    TargetRenderer(Layer *target) : LayerRenderer(target) {}

    void setup() {
        program = Box::create<LayerProgram>(Shader::fxaa());
        program->setup();

        LayerRenderer::setup();
    }

    void reset() {
        Box::release(program);

        LayerRenderer::reset();
    }

    void render() {
        LayerRenderer::render();

        program->bind(layer);
        GLuint res = glGetUniformLocation(program->program, "resolution");
        float resolution[] = {
            (float)Application::bufferWidth, (float)Application::bufferHeight
        };
        glUniform2fv(res, 1, resolution);
        program->render();
        // layer->blit();

        // TODO: MAGIC CODE
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

// class OffScreenRenderer : public BasicRenderer {
// protected:
//     Layer *layer;
//     float alpha;
//     vec3 color;
// public:
//     OffScreenRenderer(Layer *layer, float alpha, vec3 color)
//         : layer(layer), alpha(alpha), color(color) {}
//
//     void render() {
//         layer->select();
//         Layer::clear(alpha, color);
//
//         BasicRenderer::render();
//
//         Layer::detach();
//     }
// };
//
// class LayerRenderer : public OffScreenRenderer {
// public:
//     LayerRenderer() : OffScreenRenderer(NULL, 0.0f, vec3(0.0f)) {}
//
//     void setup() {
//         layer = Layer::temp();
//         OffScreenRenderer::setup();
//     }
//
//     void render() {
//         OffScreenRenderer::render();
//         layer->attach();
//     }
// };
//
// class TransparentRenderer : public BasicRenderer {
//     GLuint depth0, depth1;
// public:
//     void setup() {
//         depth0 = Texture::genDepth();
//         depth1 = Texture::genDepth();
//     }
// };

Renderer *ObjectBox::renderer() {
    return create<BasicRenderer>();
}

Renderer *ObjectBox::opaque(LightManager *light, Layer *layer) {
    return create<OpaqueRenderer>(light, layer);
}

Renderer *ObjectBox::transparent(Layer *layer) {
    return create<TransparentRenderer>(layer, 7);
}

Renderer *ObjectBox::target(Layer *layer) {
    return create<TargetRenderer>(layer);
}

// Renderer *ObjectBox::layer() {
//     return create<LayerRenderer>();
// }
