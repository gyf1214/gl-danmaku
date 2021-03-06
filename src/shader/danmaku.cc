#include "component/shader.hpp"

static GLuint program = 0;

static const char *vsh = R"(
    #version 330 core
    precision highp float;

    in vec4 time;
    in vec3 position;
    in vec3 velocity;
    in vec4 uvIndex;
    in vec3 fade;
    out float size;
    out vec4 velocityOut;
    out vec4 uvOut;
    uniform mat4 vMat;

    void main(void) {
        gl_Position = vMat * vec4(position, 1.0);
        velocityOut = vMat * vec4(velocity, 0.0);
        size = smoothstep(0.0, 1.0, fade.x);
        uvOut = uvIndex;
    }
)";

static const char *gsh = R"(
    #version 330 core
    precision highp float;

    layout(points) in;
    layout(triangle_strip, max_vertices = 4) out;

    in float size[];
    in vec4 velocityOut[];
    in vec4 uvOut[];
    out vec2 uv;

    uniform mat4 pMat;

    void main(void) {
        for (int i = 0; i < gl_in.length(); ++i) {
            if (size[i] > 0.0) {
                vec4 vPosition = gl_in[i].gl_Position;
                vec2 dir = vec2(1.0, 0.0);
                if (length(velocityOut[i].xy) > 0.0) {
                    dir = normalize(velocityOut[i].xy);
                }
                vec2 norm = vec2(-dir.y, dir.x);
                for (int j = 0; j < 4; ++j) {
                    uv = vec2(j % 2, j / 2);
                    vec2 offset = (uv - vec2(0.5)) * uvOut[i].w * size[i];
                    uv = uv * uvOut[i].z + uvOut[i].xy;
                    offset = offset.y * dir + offset.x * norm;
                    gl_Position = pMat * (vPosition + vec4(offset, 0.0, 0.0));
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
    uniform sampler2D color0;
    uniform sampler2D depth0;
    uniform sampler2D depth1;
    uniform vec2 size;

    void main(void) {
        vec4 color = texture(texture0, uv);
        if (color.a < 0.01) discard;

        vec2 coord = gl_FragCoord.xy / size;

        float d0 = texture(depth0, coord).x;
        if (gl_FragCoord.z <= d0) discard;

        float d1 = texture(depth1, coord).x;
        if (gl_FragCoord.z > d1) discard;

        vec4 c0 = texture(color0, coord);
        if (c0.a < 0.01) discard;

        fragColor = vec4(color.rgb * color.a, color.a);
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
