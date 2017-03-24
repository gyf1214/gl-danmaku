#include "shader.hpp"

static GLuint program = 0;

static const char *vsh = R"(
    #version 330 core
    precision highp float;

    in vec4 time;
    in vec3 position;
    out int show;
    uniform mat4 vMat;

    void main(void) {
        show = time.x <= 0.0 && time.y > 0.0 ? 1 : 0;
        gl_Position = vMat * vec4(position, 1.0);
    }
)";

static const char *gsh = R"(
    #version 330 core
    precision highp float;

    layout(points) in;
    layout(triangle_strip, max_vertices = 4) out;

    in int show[];
    out vec2 uv;

    uniform vec2 size;
    uniform mat4 pMat;

    void main(void) {
        for (int i = 0; i < gl_in.length(); ++i) {
            if (show[i] == 1) {
                vec4 vPosition = gl_in[i].gl_Position;
                for (int j = 0; j < 4; ++j) {
                    uv = vec2(j % 2, j / 2);
                    gl_Position = pMat * (vPosition + vec4((uv - vec2(0.5)) * size, 0.0, 0.0));
                    EmitVertex();
                }
                EndPrimitive();
            }
        }
    }
)";

static const char *fsh = R"(
    #version 330 core
    precision highp float;

    in vec2 uv;
    out vec4 fragColor;
    uniform sampler2D texture0;

    void main(void) {
        // if (gl_FragCoord.z > 1.0) discard;
        fragColor = texture(texture0, uv);
        // fragColor.x -= gl_FragCoord.z;
    }
)";

GLuint Shader::danmaku() {
    if (!program) {
        program = glCreateProgram();

        GLuint shaders[] = {
            compileShader(GL_VERTEX_SHADER, vsh),
            compileShader(GL_GEOMETRY_SHADER, gsh),
            compileShader(GL_FRAGMENT_SHADER, fsh),
        };

        for (auto shader : shaders) glAttachShader(program, shader);
        linkProgram(program);
        for (auto shader : shaders) {
            glDetachShader(program, shader);
            glDeleteShader(shader);
        }
    }
    return program;
}
