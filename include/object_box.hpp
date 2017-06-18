#ifndef __OBJECT_BOX
#define __OBJECT_BOX

#include "box.hpp"
#include "renderer.hpp"
#include "model.hpp"

class ObjectBox : public Box<Renderer> {
public:
    static Renderer *danmaku(Scene *scene, Transformer *trans);
    static Transformer *danmakuTransform(Scene *scene);
    static Renderer *skybox(Scene *scene);
    static Character *character(Scene *scene, Model *model,
                                mmd::vmd::Motion *motion, bool debug);

    static Transformer *trailTransform(Scene *scene, Character *character,
                                       int bone, vec3 pos);
    static Renderer *trail(Scene *scene, Transformer *transform,
                           float size, vec3 color);
};

#endif
