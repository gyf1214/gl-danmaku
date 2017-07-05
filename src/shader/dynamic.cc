#include "component/shader.hpp"

static GLuint program = 0;

static const char *vsh = R"(
    #version 330 core
    precision highp float;

    in vec3 position;
    in vec2 uv;

    uniform mat4 vMat;
    uniform mat4 pMat;
    uniform vec2 uvVelocity;
    uniform float frame;

    out vec2 uvOut;

    void main(void) {
        gl_Position = pMat * vMat * vec4(position, 1.0);
        uvOut = uv + frame * uvVelocity;
    }
)";

static const char *fsh = R"(
    #version 330 core
    precision highp float;

    in vec2 uvOut;

    uniform sampler2D texture0;

    out vec4 fragColor;

    void main(void) {
        fragColor = texture(texture0, uvOut);
    }
)";

GLuint Shader::dynamic() {
    if (!program) program = programShader(vsh, fsh);
    return program;
}
