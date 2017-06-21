#include "ext.hpp"

using namespace std;

class BasicRenderer : public virtual Renderer {
protected:
    vector<Object *> objects;
public:
    void setup() { for (const auto &o : objects) o->setup(); }
    void reset() { for (const auto &o : objects) Box::release(o); objects.clear(); }
    void render() { for (const auto &o : objects) o->render(); }
    void push(Object *o) { objects.push_back(o); }
};

class OpaqueRenderer : public BasicRenderer {
    LightManager *light;
public:
    OpaqueRenderer(LightManager *light) : light(light) {}

    void render() {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        light->pass() = 0;
        BasicRenderer::render();

        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        glBlendEquation(GL_FUNC_ADD);

        int n = light->count();
        for (int i = 1 ; i < n; ++i) {
            light->pass() = i;
            BasicRenderer::render();
        }

        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
        glDisable(GL_DEPTH_TEST);
    }
};

class OffScreenRenderer : public BasicRenderer {
protected:
    Layer *layer;
    float alpha;
    vec3 color;
public:
    OffScreenRenderer(Layer *layer, float alpha, vec3 color)
        : layer(layer), alpha(alpha), color(color) {}

    void render() {
        layer->select();
        Layer::clear(alpha, color);

        BasicRenderer::render();

        Layer::detach();
    }
};

class LayerRenderer : public OffScreenRenderer {
public:
    LayerRenderer() : OffScreenRenderer(NULL, 0.0f, vec3(0.0f)) {}

    void setup() {
        layer = Layer::temp();
        OffScreenRenderer::setup();
    }

    void render() {
        OffScreenRenderer::render();
        layer->attach();
    }
};

Renderer *ObjectBox::renderer() {
    return create<BasicRenderer>();
}

Renderer *ObjectBox::opaque(LightManager *light) {
    return create<OpaqueRenderer>(light);
}

Renderer *ObjectBox::layer() {
    return create<LayerRenderer>();
}
