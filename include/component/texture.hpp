#ifndef __TEXTURE
#define __TEXTURE

#include "common.hpp"

struct Texture {
    static GLuint genTexture(GLuint inter = GL_LINEAR,
                             GLuint wraps = GL_CLAMP_TO_BORDER,
                             GLuint wrapt = GL_CLAMP_TO_BORDER);
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
    static GLuint trail();
    static GLuint wall1_ref();
    static GLuint wall1_nor();
    static GLuint wall1_emi();
    static GLuint wall2_ref();
    static GLuint wall2_nor();
    static GLuint wall2_emi();
    static GLuint lavaflow();

    static void release();
};

#endif
