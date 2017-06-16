#include "shader.hpp"

static GLuint program = 0;

static const char *vsh = R"(
    #version 330 core
    precision highp float;

    in vec3 position;
    in float alpha;

    uniform mat4 vMat;
    uniform mat4 pMat;

    out float alphaOut;

    void main(void) {
        gl_Position = pMat * vMat * vec4(position, 1.0);
        gl_PointSize = 100.0 * (0.5 + 0.5 * alpha) / gl_Position.w;
        alphaOut = alpha;
    }
)";

static const char *fsh = R"(
    #version 330 core
    precision highp float;

    in float alphaOut;
    out vec4 fragColor;

    uniform sampler2D texture0;

    void main(void) {
        fragColor = texture(texture0, gl_PointCoord);
        fragColor.a *= alphaOut;
    }
)";

GLuint Shader::trail() {
    if (!program) program = programShader(vsh, fsh);
    return program;
}
