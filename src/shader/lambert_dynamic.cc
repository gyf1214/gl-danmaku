#include "component/shader.hpp"

static GLuint program = 0;

static const char *vsh = R"(
    #version 330 core
    precision highp float;

    in vec3 position;
    in vec3 normal;
    in vec3 tangent;
    in vec2 uv;

    uniform mat4 vMat;
    uniform mat4 pMat;
    uniform vec4 lightPosition;
    uniform int frame;
    uniform vec2 uvVelocity;

    out vec3 normalOut;
    out vec3 tangentOut;
    out vec2 uvOut;
    out vec4 vPos;
    out vec4 lPos;

    void main(void) {
        vPos = vMat * vec4(position, 1.0);
        gl_Position = pMat * vPos;

        normalOut = normalize((vMat * vec4(normal, 0.0)).xyz);
        tangentOut = normalize((vMat * vec4(tangent, 0.0)).xyz);
        uvOut = uv + frame * uvVelocity;

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
    in vec3 tangentOut;
    in vec4 vPos;
    in vec4 lPos;
    in vec2 uvOut;

    uniform vec3 lightColor;
    uniform vec3 ambient;
    uniform vec4 lightMaterial;
    uniform vec4 material;
    uniform sampler2D diffuse;
    uniform sampler2D normal;
    uniform sampler2D specular;
    uniform sampler2D emission;

    out vec4 fragColor;

    void main(void) {
        vec3 color = texture(diffuse, uvOut).rgb;
        vec3 c = ambient * color * material.x;

        vec3 N = normalize(normalOut);
        vec3 L = normalize(lPos.xyz);
        vec3 H = normalize(L - normalize(vPos.xyz));

        if (length(tangentOut) > 0.0) {
            vec3 T = normalize(tangentOut);
            vec3 B = cross(N, T);
            vec3 map = 2.0 * texture(normal, uvOut).xyz - 1.0;
            N = normalize(map.x * T + map.y * B + map.z * N);
        }

        float scalar = 1.0;
        if (lPos.w != 0.0) {
            float dis = length(lPos.xyz);
            scalar /= dot(lightMaterial.xyz, vec3(1.0, dis, dis * dis));
        }

        float diff = scalar * material.x * max(dot(L, N), 0.0);
        c += lightColor * color * diff;

        float spec = scalar * material.y * pow(max(dot(N, H), 0.0), material.w);
        color = texture(specular, uvOut).rgb;
        c += lightColor * color * spec;

        color = texture(emission, uvOut).rgb;
        c += material.z * lightMaterial.w * color;

        fragColor = vec4(c, 1.0);
    }
)";

GLuint Shader::lambert_dynamic() {
    if (!program) program = programShader(vsh, fsh);
    return program;
}
