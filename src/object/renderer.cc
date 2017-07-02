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

        Layer::detach();
    }
};

class TargetRenderer : public LayerRenderer {
public:
    TargetRenderer(Layer *target) : LayerRenderer(target) {}

    void render() {
        LayerRenderer::render();

        layer->blit();
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

Renderer *ObjectBox::target(Layer *layer) {
    return create<TargetRenderer>(layer);
}

// Renderer *ObjectBox::layer() {
//     return create<LayerRenderer>();
// }
