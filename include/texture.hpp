#ifndef __TEXTURE
#define __TEXTURE

#include <GL/glew.h>

class Texture {
    static GLuint loadTexture(const char *name);
public:
    static GLuint small();
    static GLuint etama();
};

#endif
