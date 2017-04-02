#ifndef __SHADER
#define __SHADER

#include "GL/glew.h"

class Shader {
protected:
    static GLuint compileShader(GLenum type, const char *src);
    static void linkProgram(GLuint program);
    static GLuint programShader(const char *vshSrc, const char *fshSrc);
public:
    static GLuint danmaku(void);
    static GLuint danmakuTransform(void);
    static GLuint base(void);
    static GLuint lambert(void);
};

#endif
