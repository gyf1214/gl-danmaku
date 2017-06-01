#include "shader.hpp"

static GLuint program = 0;

static const char *vsh = R"(
    #version 330 core
    precision highp float;

    in vec3 position;
    in vec3 normal;
    in vec2 uv;
    in ivec4 boneIndex;
    in vec4 boneWeight;

    uniform mat4 mMat;
    uniform mat4 vMat;
    uniform mat4 pMat;
    uniform vec4 lightPosition;

    uniform Bones {
        mat4 bones[256];
    };

    out vec3 normalOut;
    out vec2 uvOut;
    out vec4 vPos;
    out vec4 lPos;

    void main(void) {
        mat4 bMat = boneWeight.x * bones[boneIndex.x] + boneWeight.y * bones[boneIndex.y];
        vPos = vMat * mMat * bMat * vec4(position, 1.0);
        gl_Position = pMat * vPos;

        normalOut = normalize((vMat * vec4(normal, 0.0)).xyz);
        uvOut = vec2(uv.x, 1.0 - uv.y);

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
    uniform float side;
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

        vec3 diff = scalar * diffuse.xyz * max(dot(L, N) * side, 0.0);
        c += lightColor * color * diff;

        vec3 spec = scalar * specular.xyz * pow(max(dot(N, H), 0.0), specular.w);
        c += lightColor * spec;

        fragColor = vec4(c, 1.0);
    }
)";

GLuint Shader::character() {
    if (!program) program = programShader(vsh, fsh);
    return program;
}
