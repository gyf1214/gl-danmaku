#include "shader.hpp"

static GLuint program = 0;

static const char *vsh = R"(
    #version 330 core
    precision highp float;

    in vec4 time;
    in vec3 position;
    in vec2 uv;
    out vec2 uvOut;
    uniform vec2 size;
    uniform mat4 vMat;
    uniform mat4 pMat;

    void main(void) {
        vec4 t = time;
        vec4 vPosition = vMat * vec4(position, 1.0);
        vec2 oPosition = (uv - vec2(0.5)) * size;
        vPosition += vec4(oPosition, 0.0, 0.0);
        gl_Position = pMat * vPosition;
        uvOut = uv;
    }
)";

static const char *fsh = R"(
    #version 330 core
    precision highp float;

    in vec2 uvOut;
    out vec4 fragColor;
    uniform sampler2D texture0;

    void main(void) {
        // if (gl_FragCoord.z > 1.0) discard;
        fragColor = texture(texture0, uvOut);
        // fragColor.x -= gl_FragCoord.z;
    }
)";

GLuint Shader::danmaku() {
    if (!program) program = programShader(vsh, fsh);
    return program;
}
