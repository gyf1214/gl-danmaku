#include "shader.hpp"

static GLuint program = 0;

const char *varyings[] = {
    "time",
    "position",
    "velocity",
    "acceleration",
    "uvIndex",
};

static const char *vsh = R"(
    #version 330 core
    precision highp float;

    in vec4 time0;
    in vec3 position0;
    in vec3 velocity0;
    in vec4 acceleration0;
    in vec4 uvIndex0;
    out vec4 time;
    out vec3 position;
    out vec3 velocity;
    out vec4 acceleration;
    out vec4 uvIndex;

    const float elapse = 1.0 / 60.0;

    void main() {
        time = time0 - vec4(elapse);
        acceleration = acceleration0;
        uvIndex = uvIndex0;
        vec3 accel;
        if (acceleration0.w == 0.0) {
            accel = acceleration0.xyz;
        } else {
            accel = normalize(acceleration0.xyz - position0) * acceleration0.w;
        }
        if (time0.z <= 0.0 && time0.w > 0.0) {
            velocity = velocity0 + accel * elapse;
            position = position0 + (velocity0 + velocity) * 0.5 * elapse;
        } else {
            velocity = velocity0;
            if (time0.z < elapse) {
                velocity += accel * (elapse - time0.z);
            }
            position = position0;
            if (time0.z < elapse) {
                position += (velocity0 + velocity) * 0.5 * (elapse - time0.z);
            }
        }
    }
)";

GLuint Shader::danmakuTransform() {
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
