#include "component/shader.hpp"

static GLuint program = 0;

static const char *varyings[] = {
    "position",
    "alpha"
};

static const char *vsh = R"(
    #version 330 core
    precision highp float;

    in vec3 position0;
    in float alpha0;
    out vec3 position;
    out float alpha;

    uniform vec3 position1;
    uniform vec3 position2;
    uniform float alpha1;
    uniform float alpha2;
    uniform float elapse;

    void main() {
        if (alpha0 < 0.0) {
            position = mix(position2, position1, -alpha0);
            alpha = mix(alpha2, alpha1, -alpha0);
        } else {
            position = position0;
            alpha = max(alpha0 - elapse, 0.0);
        }
    }
)";

GLuint Shader::trailTransform() {
    if (!program) {
        program = glCreateProgram();
        GLuint shader = compileShader(GL_VERTEX_SHADER, vsh);
        glAttachShader(program, shader);
        int size = sizeof(varyings) / sizeof(varyings[0]);
        glTransformFeedbackVaryings(program, size, varyings, GL_INTERLEAVED_ATTRIBS);

        linkProgram(program);

        glDetachShader(program, shader);
        glDeleteShader(shader);
    }
    return program;
}
