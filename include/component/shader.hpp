#ifndef __SHADER
#define __SHADER

#include "common.hpp"

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
    static GLuint character(void);
    static GLuint trail(void);
    static GLuint trailTransform(void);
    static GLuint debug(void);
    static GLuint layer(void);
    // static GLuint layerDiscard(void);
    static GLuint fxaa(void);

    static void release(void);
};

#endif
