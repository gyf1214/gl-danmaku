#include "component/shader.hpp"

static GLuint program = 0;

static const char *vsh = R"(
    #version 330 core
    precision highp float;

    in vec3 position;
    in vec3 color;

    uniform mat4 mat;

    out vec3 colorOut;

    void main(void) {
        gl_Position = mat * vec4(position, 1.0);
        colorOut = color;
    }
)";

static const char *fsh = R"(
    #version 330 core
    precision highp float;

    in vec3 colorOut;
    out vec4 fragColor;

    void main(void) {
        fragColor = vec4(colorOut, 1.0);
    }
)";

GLuint Shader::debug() {
    if (!program) program = programShader(vsh, fsh);
    return program;
}
