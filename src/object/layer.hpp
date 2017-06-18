#ifndef __LAYER
#define __LAYER

#include "ext.hpp"

class Layer {
    GLuint framebuffer;
    GLuint color, depth;
    Layer();
public:
    void select(void);
    void render(void);

    static void detach(void);
    static void clear(void);
    static void setup(void);
    static Layer *temp(void);
};

#endif
