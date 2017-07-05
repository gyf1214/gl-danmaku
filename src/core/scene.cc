#include "../ext.hpp"
#include "object_box.hpp"

class BasicScene : public virtual Scene {
    Renderer *root;
    Script *script;
public:
    BasicScene(Script *script) : script(script) {}
    void setup() {
        root = script->createObjects();
        script->setup();
    }
    void reset() { script->reset(); }
    void update() { script->update(); }
    void render() { root->render(); }
};

Scene *ObjectBox::scene(Script *script) {
    return create<BasicScene>(script);
}
