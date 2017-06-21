#include "../ext.hpp"
#include "object_box.hpp"

class BasicScene : public virtual Scene {
    Renderer *root;
    Script *script;
public:
    BasicScene(Script *script) : root(ObjectBox::renderer()), script(script) {}
    ~BasicScene() { Box::release(root); }
    void setup() {
        script->createObjects(root);
        script->setup();
        root->setup();
    }
    void reset() {
        root->reset();
        script->reset();
    }
    void update() { script->update(); }
    void render() { root->render(); }
};

Scene *ObjectBox::scene(Script *script) {
    return create<BasicScene>(script);
}
