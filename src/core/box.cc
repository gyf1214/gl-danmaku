#include "../ext.hpp"
#include "component/texture.hpp"
#include "component/shader.hpp"
#include "component/layer.hpp"

using namespace std;

void *Box::alloc(size_t n) {
    return malloc(n);
}

void Box::dealloc(void *o) {
    free(o);
}

void Box::release(BaseObject *o)  {
    if (o) {
        o->reset();
        o->~BaseObject();
        dealloc(o);
    }
}

static vector<BaseObject *> globals;

void Box::track(BaseObject *o) {
    globals.push_back(o);
}

void Box::releaseGlobal() {
    int n = globals.size();
    for (int i = 0; i < n; ++i) {
        release(globals[i]);
    }
    globals.clear();
    Layer::release();
    Texture::release();
    Shader::release();
}
