#include "component/shader.hpp"

static GLuint program = 0;

static const char *varyings[] = {
    "time",
    "position",
    "velocity",
    "acceleration",
    "uvIndex",
    "fade",
};

static const char *vsh = R"(
    #version 330 core
    precision highp float;

    in vec4 time0;
    in vec3 position0;
    in vec3 velocity0;
    in vec4 acceleration0;
    in vec4 uvIndex0;
    in vec3 fade0;
    out vec4 time;
    out vec3 position;
    out vec3 velocity;
    out vec4 acceleration;
    out vec4 uvIndex;
    out vec3 fade;

    uniform float elapse;

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

        float ela = clamp(elapse - time0.z, 0.0, elapse);
        ela = clamp(time0.w, 0.0, ela);

        velocity = velocity0 + accel * ela;
        position = position0 + (velocity0 + velocity) * 0.5 * ela;

        fade = fade0;

        ela = clamp(elapse - time0.x, 0.0, elapse);
        ela = clamp(time0.y, 0.0, ela);
        // ela = 1.0;
        fade.x = fade.x + fade0.y * ela;

        ela = clamp(-time0.y, 0.0, elapse);
        // ela = 0.0;
        fade.x = fade.x - fade0.z * ela;

        fade.x = clamp(fade.x, 0.0, 1.0);
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
