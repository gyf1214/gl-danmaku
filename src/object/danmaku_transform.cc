#include "transform_renderer.hpp"
#include "vertex/danmaku.hpp"
#include "builder.hpp"

static Vertex vertexData[vertexSize];

proto(DanmakuTransform, Shader::danmakuTransform);

protoBuffer = {
    { sizeof(vertexData), vertexData },
    { sizeof(vertexData), NULL }
};

protoAttrib = {
    { "time0"        , Offset(Vertex, time[0])        , 4, sizeof(Vertex) },
    { "position0"    , Offset(Vertex, position[0])    , 3, sizeof(Vertex) },
    { "velocity0"    , Offset(Vertex, velocity[0])    , 3, sizeof(Vertex) },
    { "acceleration0", Offset(Vertex, acceleration[0]), 4, sizeof(Vertex) },
    { "uvIndex0"     , Offset(Vertex, uvIndex[0])     , 4, sizeof(Vertex) },
};

protoUnifom = {};

using namespace Builder;

static void setupVertices() {
    LOG << "setup vertices";
    srand(time(NULL));

    Base *src = source(vertexData, vertexSize) -> set();

    // Chain(generator(10))
    //     << line(vec3(-2.0f, 5.0f, 20.0f),
    //             vec3(4.0f, 0.0f, 0.0f) / 5.0f)
    //     << emitter(7.0f, 0.1f)
    //     << type(0, 24, 8, 1.0f)
    //     << direction(0.0f, 0.0f, -1.0f)
    //     << generator(20)
    //     << linearSpeed(0.5f, 0.5f)
    //     << dieAfter(60.0f)
    //     << src << Emit();
    //
    // Chain(generator(1))
    //     << point(0.0f, -5.0f, 20.0f)
    //     << sphere(5, 5)
    //     << generator(10)
    //     << emitter(3.0f, 0.1f)
    //     << type(0, 16, 8, 0.3f)
    //     << linearSpeed(3.0f, 0.0f)
    //     << dieAfter(3.0f)
    //     << src << Emit();
    //
    // Chain(generator(1))
    //     << point(0.0f, 0.0f, 20.0f)
    //     << direction(5.0f, 0.0f, 0.0f)
    //     << circleMotion(0.0f, 10.0f, 20.0f)
    //     << generator(20)
    //     << emitter(2.0f, 0.05f)
    //     << type(0, 16, 8, 0.3f)
    //     << src << Emit();

    Multi circles;
    const int N = 3;
    const int M = 3;

    for (int k = -M; k <= M; ++k) {
        float pitch = (float)k / (float)(M + 1) * M_PI / 2.0f;
        int T = 4 * (1 + floor(N * cos(pitch)));
        for (int i = 0; i < T; ++i) {
            float angle = (float)i / (float)T * M_PI * 2.0f;
            vec3 x = vec3(cos(angle) * cos(pitch),
                        sin(angle) * cos(pitch), 0.0f) * 10.0f;
            vec3 y = vec3(-sin(angle) * cos(pitch),
                        cos(angle) * cos(pitch), 0.0f) * 10.0f;

            Multi m = Multi()
                << circle(x, y,  0.03f)
                << circle(x, y, -0.03f);
            Chain c = Chain(m)
                << addVelocity(vec3(0.0f, 0.0f, sin(pitch)) * 10.0f);
            circles << c;
        }
    }

    //
    // Chain(generator(1))
    //     << point(0.0f, 0.0f, 20.0f)
    //     << circle << generator(30)
    //     << emitter(3.0f, 0.05f)
    //     << dieAfter(2.0f)
    //     << type(0, 16, 8, 0.6f)
    //     << src << Emit();

    Chain(generator(50))
        << point(0.0f, 0.0f, 20.0f)
        << emitter(3.0f, 0.02f)
        << circles << dieAfter(2.0f)
        << type(0, 16, 8, 0.6f)
        << src << Emit();

    // Multi bias = Multi()
    //     << crossBias(0.0f, 0.0f,  1.0f);
    //     << crossBias(0.0f, 0.0f, -10.0f);
    //
    // Chain(generator(1))
    //     << point(0.0f, 0.0f, 20.0f)
    //     << direction(5.0f, 0.0f, 0.0f)
    //     << sphere(5, 5)
    //     << bias
    //     << generator(30)
    //     << emitter(2.0f, 0.05f)
    //     << type(0, 16, 8, 0.3f)
    //     << src << Emit();

    src -> reset();

    LOG << "done setup vertices";
}

class DanmakuTransform : public TransformRenderer<Proto> {
public:
    DanmakuTransform(Scene *scene) : TransformRenderer(scene) {}

    void setup() {
        setupVertices();

        TransformRenderer::setup();
    }

    void update() {
        std::swap(buffer[0], buffer[1]);
        bindProgram();
        bindBuffer(buffer[1]);

        begin(GL_POINTS);
        glDrawArrays(GL_POINTS, 0, vertexSize);
        end();
    }

    GLuint outputBuffer() { return buffer[0]; }
};

Transformer *ObjectBox::danmakuTransform(Scene *scene) {
    return new DanmakuTransform(scene);
}
