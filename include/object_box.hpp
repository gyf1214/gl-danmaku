#ifndef __OBJECT_BOX
#define __OBJECT_BOX

#include "box.hpp"
#include "renderer.hpp"

class ObjectBox : public Box<Renderer> {
public:
    static Renderer *danmaku(Scene *scene, Transformer *trans);
    static Transformer *danmakuTransform(Scene *scene);
};

#endif
