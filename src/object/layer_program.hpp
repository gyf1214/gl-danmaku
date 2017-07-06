#ifndef __EXT_LAYER_PROGRAM
#define __EXT_LAYER_PROGRAM

#include "ext.hpp"

class LayerProgram : public virtual Object {
public:
    GLuint program, vao, uv, color, depth;

    LayerProgram(GLuint program);

    void setup(void);
    void reset(void);
    void bind(Layer *layer);
    void render(void);
};

#endif
