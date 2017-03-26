#ifndef __TEXTURE
#define __TEXTURE

#include <GL/glew.h>

#define defineTexture(name, path) static GLuint tex_##name = 0;\
GLuint Texture::name() {\
    if (!tex_##name) tex_##name = loadTexture(path);\
    return tex_##name;\
}

class Texture {
    static GLuint loadTexture(const char *name);
public:
    static GLuint small();
    static GLuint etama();
};

#endif
