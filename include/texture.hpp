#ifndef __TEXTURE
#define __TEXTURE

#include <GL/glew.h>

class Texture {
    static GLuint loadTexture(const char *name, GLint wrap);
public:
    static GLuint small();
    static GLuint etama();
    static GLuint rockNormal();
    static GLuint sceneWall();
    static GLuint sceneFloor();
};

#endif
