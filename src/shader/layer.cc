#include "component/shader.hpp"

static const char *vsh = R"(
    #version 330 core
    precision highp float;

    in vec2 uv;
    out vec2 uvOut;

    void main(void) {
        vec2 pos = 2.0 * uv - 1.0;
        gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
        uvOut = uv;
    }
)";

static const char *fsh = R"(
    #version 330 core
    precision highp float;

    in vec2 uvOut;

    uniform sampler2D color;
    uniform sampler2D depth;

    out vec4 fragColor;

    void main(void) {
        vec4 c = texture(color, uvOut);
        // if (c.a < 0.01) discard;
        fragColor = c;
        gl_FragDepth = texture(depth, uvOut).r;
    }
)";

static const char *fsh_transparent = R"(
    #version 330 core
    precision highp float;

    in vec2 uvOut;

    uniform sampler2D color;
    uniform sampler2D depth;
    uniform sampler2D color0;
    uniform sampler2D depth0;
    uniform sampler2D depth1;

    out vec4 fragColor;

    void main(void) {
        vec4 color = texture(color, uvOut);
        if (color.a < 0.01) discard;

        float d = texture(depth, uvOut).r;

        float d0 = texture(depth0, uvOut).x;
        if (d <= d0) discard;

        float d1 = texture(depth1, uvOut).x;
        if (d > d1) discard;

        vec4 c0 = texture(color0, uvOut);
        if (c0.a < 0.01) discard;

        fragColor = vec4(color.rgb * color.a, color.a);
        gl_FragDepth = d;
    }
)";

static const char *fsh_fxaa = R"(
    #version 330 core
    precision highp float;

    in vec2 uvOut;

    uniform sampler2D color;
    uniform sampler2D depth;

    uniform vec2 resolution;

    out vec4 fragColor;

    const vec3 luma = vec3(0.299, 0.587, 0.114);
    const float FXAA_REDUCE_MIN = 1.0 / 128.0;
    const float FXAA_REDUCE_MUL = 1.0 / 8.0;
    const float FXAA_SPAN_MAX = 8.0;

    void main(void) {
        vec2 iVP = 1.0 / resolution;

        vec2 v_rgbNW = uvOut + vec2(-1.0, -1.0) * iVP;
    	vec2 v_rgbNE = uvOut + vec2( 1.0, -1.0) * iVP;
    	vec2 v_rgbSW = uvOut + vec2(-1.0,  1.0) * iVP;
    	vec2 v_rgbSE = uvOut + vec2( 1.0,  1.0) * iVP;
    	vec2 v_rgbM = uvOut;

        vec3 rgbNW = texture(color, v_rgbNW).rgb;
        vec3 rgbNE = texture(color, v_rgbNE).rgb;
        vec3 rgbSW = texture(color, v_rgbSW).rgb;
        vec3 rgbSE = texture(color, v_rgbSE).rgb;
        vec3 rgbM = texture(color, v_rgbM).rgb;

        float lumaNW = dot(rgbNW, luma);
        float lumaNE = dot(rgbNE, luma);
        float lumaSW = dot(rgbSW, luma);
        float lumaSE = dot(rgbSE, luma);
        float lumaM  = dot(rgbM,  luma);
        float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
        float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

        vec2 dir;
        dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
        dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));

        float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) *
                          (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);

        float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
        dir = clamp(dir * rcpDirMin, -FXAA_SPAN_MAX, FXAA_SPAN_MAX) * iVP;

        vec3 rgbA = 0.5 * (
            texture(color, uvOut + dir * (1.0 / 3.0 - 0.5)).rgb +
            texture(color, uvOut + dir * (2.0 / 3.0 - 0.5)).rgb);

        vec3 rgbB = rgbA * 0.5 + 0.25 * (
            texture(color, uvOut + dir * -0.5).rgb +
            texture(color, uvOut + dir *  0.5).rgb);

        float lumaB = dot(rgbB, luma);
        if ((lumaB < lumaMin) || (lumaB > lumaMax)) {
            fragColor = vec4(rgbA, 1.0);
        } else {
            fragColor = vec4(rgbB, 1.0);
        }

        gl_FragDepth = texture(depth, uvOut).x;
    }
)";

GLuint Shader::layer() {
    static GLuint program = 0;
    if (!program) program = programShader(vsh, fsh);
    return program;
}

GLuint Shader::fxaa() {
    static GLuint program = 0;
    if (!program) program = programShader(vsh, fsh_fxaa);
    return program;
}

GLuint Shader::layerTransparent() {
    static GLuint program = 0;
    if (!program) program = programShader(vsh, fsh_transparent);
    return program;
}
