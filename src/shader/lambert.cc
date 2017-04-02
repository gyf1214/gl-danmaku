#include "shader.hpp"

static GLuint program = 0;

static const char *vsh = R"(
    #version 330 core
    precision highp float;

    in vec3 position;
    in vec3 normal;
    in vec2 uv;

    uniform mat4 vMat;
    uniform mat4 pMat;
    uniform vec4 lightPosition;

    out vec3 normalOut;
    out vec2 uvOut;
    out vec4 vPos;
    out vec4 lPos;

    void main(void) {
        vPos = vMat * vec4(position, 1.0);
        gl_Position = pMat * vPos;

        normalOut = normalize((vMat * vec4(normal, 0.0)).xyz);
        uvOut = uv;

        lPos = vMat * lightPosition;
        if (lPos.w == 0.0) {
            lPos = vec4(normalize(-lPos.xyz), 0.0);
        } else {
            lPos = vec4(lPos.xyz - vPos.xyz, 1.0);
        }
    }
)";

static const char *fsh = R"(
    #version 330 core
    precision highp float;

    in vec3 normalOut;
    in vec4 vPos;
    in vec4 lPos;
    in vec2 uvOut;

    uniform vec3 lightColor;
    uniform vec3 ambient;
    uniform sampler2D texture0;
    uniform vec2 material;

    out vec4 fragColor;

    void main(void) {
        vec3 color = texture(texture0, uvOut).rgb;
        vec3 c = ambient * color * material.x;

        vec3 N = normalize(normalOut);
        vec3 L = normalize(lPos.xyz);
        vec3 H = normalize(lPos.xyz - vPos.xyz);

        if (dot(N, vPos.xyz) > 0) discard;

        float scalar = 1.0;
        if (lPos.w != 0.0) {
            float dis = length(lPos.xyz);
            scalar /= dis * dis;
        }

        float diffuse = scalar * max(dot(L, N), 0.0);
        float specular = scalar * pow(max(dot(N, H), 0.0), 20.0);

        c += lightColor * color * diffuse * material.x;
        c += specular * lightColor * material.y;

        fragColor = vec4(c, 1.0);
    }
)";

GLuint Shader::lambert() {
    if (!program) program = programShader(vsh, fsh);
    return program;
}
