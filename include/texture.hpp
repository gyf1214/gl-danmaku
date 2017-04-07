#ifndef __TEXTURE
#define __TEXTURE

#include <GL/glew.h>

class Texture {
    static GLuint loadTexture(const char *name, GLuint wraps, GLuint wrapt);
public:
    static GLuint small();
    static GLuint etama();
    static GLuint wallNormal();
    static GLuint wall();
    static GLuint lava();
    static GLuint wallLava();
    static GLuint white();
};

#endif
