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

Renderer *ObjectBox::renderer() {
    return create<BasicRenderer>();
}

Renderer *ObjectBox::opaque(LightManager *light) {
    return create<OpaqueRenderer>(light);
}
