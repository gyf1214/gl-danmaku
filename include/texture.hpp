#ifndef __TEXTURE
#define __TEXTURE

#include "common.hpp"

struct Texture {
    static GLuint loadTexture(const char *name,
        GLuint wraps = GL_CLAMP_TO_BORDER,
        GLuint wrapt = GL_CLAMP_TO_BORDER);
    static GLuint small();
    static GLuint etama();
    static GLuint wallNormal();
    static GLuint wall();
    static GLuint lava();
    static GLuint wallLava();
    static GLuint white();
    static GLuint wall1();
    static GLuint wall2();
};

#endif
