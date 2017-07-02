#include "component/shader.hpp"

static GLuint program = 0;

static const char *vsh = R"(
    #version 330 core
    precision highp float;

    in vec2 uv;
    out vec2 uvOut;

    void main(void) {
        vec2 pos = 2.0 * uv - 1.0;
        gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
        uvOut = uv;
    }
)";

static const char *fsh = R"(
    #version 330 core
    precision highp float;

    in vec2 uvOut;

    uniform sampler2D color;
    uniform sampler2D depth;

    out vec4 fragColor;

    void main(void) {
        vec4 c = texture(color, uvOut);
        if (c.a < 0.01) discard;
        fragColor = c;
        gl_FragDepth = texture(depth, uvOut).r;
    }
)";

GLuint Shader::layer() {
    if (!program) program = programShader(vsh, fsh);
    return program;
}
