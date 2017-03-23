#include "shader.hpp"

static GLuint program = 0;

const char *varyings[] = {
    "time",
    "position",
    "velocity",
    "acceleration",
};

static const char *vsh = R"(
    #version 330 core
    precision highp float;

    in vec4 time0;
    in vec3 position0;
    in vec3 velocity0;
    in vec3 acceleration0;
    out vec4 time;
    out vec3 position;
    out vec3 velocity;
    out vec3 acceleration;

    const float elapse = 1.0 / 60.0;

    void main() {
        time = time0 - vec4(elapse);
        acceleration = acceleration0;
        if (time0.z <= 0.0 && time0.w >= 0) {
            velocity = velocity0 + acceleration0 * elapse;
            position = position0 + (velocity0 + velocity) * 0.5 * elapse;
        } else {
            velocity = velocity0;
            position = position0;
        }
    }
)";

GLuint Shader::danmakuTransform() {
    if (!program) {
        program = glCreateProgram();
        GLuint shader = compileShader(GL_VERTEX_SHADER, vsh);
        glAttachShader(program, shader);
        glTransformFeedbackVaryings(program, 4, varyings, GL_INTERLEAVED_ATTRIBS);

        linkProgram(program);

        glDetachShader(program, shader);
        glDeleteShader(shader);
    }
    return program;
}
