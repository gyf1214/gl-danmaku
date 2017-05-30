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
    in vec2 uvOut;
    in vec4 vPos;
    in vec4 lPos;

    uniform vec3 lightColor;
    uniform vec3 ambient;
    uniform vec4 lightMaterial;
    uniform vec4 diffuse;
    uniform vec4 specular;
    uniform sampler2D texture0;

    out vec4 fragColor;

    void main(void) {
        vec3 color = texture(texture0, uvOut).rgb;
        vec3 c = ambient * color;

        vec3 N = normalize(normalOut);
        vec3 L = normalize(lPos.xyz);
        vec3 H = normalize(L - normalize(vPos.xyz));

        float scalar = 1.0;
        if (lPos.w != 0.0) {
            float dis = length(lPos.xyz);
            scalar /= dot(lightMaterial.xyz, vec3(1.0, dis, dis * dis));
        }

        vec3 diff = scalar * diffuse.xyz * max(dot(L, N), 0.0);
        c += lightColor * color * diff;

        vec3 spec = scalar * specular.xyz * pow(max(dot(N, H), 0.0), specular.w);
        c += lightColor * spec;

        fragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
)";

GLuint Shader::character() {
    if (!program) program = programShader(vsh, fsh);
    return program;
}
