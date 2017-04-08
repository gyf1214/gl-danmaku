#include <GL/glew.h>
#include <glm/glm.hpp>
#include <algorithm>
#include "transform_renderer.hpp"
#include "vertex/danmaku.hpp"
#include "object_box.hpp"
#include "shader.hpp"
#include "util.hpp"
#include "builder.hpp"

static Vertex vertexData[vertexSize];

transProto(DanmakuTransform, Shader::danmakuTransform);

protoBuffer = {
    { sizeof(vertexData), vertexData }
};

protoOutput = { sizeof(vertexData), NULL };

protoAttrib = {
    { "time0"        , Offset(Vertex, time[0])        , 4, sizeof(Vertex) },
    { "position0"    , Offset(Vertex, position[0])    , 3, sizeof(Vertex) },
    { "velocity0"    , Offset(Vertex, velocity[0])    , 3, sizeof(Vertex) },
    { "acceleration0", Offset(Vertex, acceleration[0]), 3, sizeof(Vertex) },
    { "uvIndex0"     , Offset(Vertex, uvIndex[0])     , 4, sizeof(Vertex) },
};

protoUnifom = {};

using namespace Builder;

static void setupVertices() {
    LOG << "setup vertices";
    srand(time(NULL));

    Base *src = source(vertexData, vertexSize) -> set();
    Multi dirs = Multi() << direction( 5.0f,  0.0f,  0.0f)
                         << direction( 0.0f,  5.0f,  0.0f)
                         << direction( 0.0f,  0.0f,  5.0f)
                         << direction(-5.0f,  0.0f,  0.0f)
                         << direction( 0.0f, -5.0f,  0.0f)
                         << direction( 0.0f,  0.0f, -5.0f);

    Chain(generator(10))
        << emitter(0.0f, 0.1f)
        << point(2.0f, 0.0f, 5.0f)
        << type(12, 0, 1, 0.2f) << dirs << src << Emit();

    Chain(generator(10))
        << emitter(0.5f, 0.1f)
        << point(-2.0f, 0.0f, 5.0f)
        << type(12, 0, 1, 0.2f) << dirs << src << Emit();

    Multi tars = Multi()
        << targetNorm(glm::vec3(-5.0f, -5.0f, 7.0f), 10.0f)
        << targetNorm(glm::vec3(0.0f, -5.0f, 7.0f), 10.0f)
        << targetNorm(glm::vec3(5.0f, -5.0f, 7.0f), 10.0f)
        << targetNorm(glm::vec3(-5.0f, -5.0f, 3.0f), 10.0f)
        << targetNorm(glm::vec3(0.0f, -5.0f, 3.0f), 10.0f)
        << targetNorm(glm::vec3(5.0f, -5.0f, 3.0f), 10.0f);

    Chain(generator(5))
        << line(glm::vec3(-2.0f, 4.0f, 5.0f),
                glm::vec3( 4.0f, 0.0f, 0.0f) / 5.0f)
        << generator(20)
        << emitter(2.0f, 0.1f)
        << tars
        << type(14, 2, 1, 0.2f) << src << Emit();

    Chain(generator(10))
        << line(glm::vec3(-2.0f, 5.0f, 20.0f),
                glm::vec3(4.0f, 0.0f, 0.0f) / 5.0f)
        << emitter(3.0f, 0.1f)
        << type(0, 24, 8, 1.0f)
        << direction(0.0f, 0.0f, -1.0f)
        << generator(20)
        << linearSpeed(0.1f, 0.5f)
        << src << Emit();

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
        bindProgram();
        bindBuffer(buffer[0]);

        begin(GL_POINTS);
        glDrawArrays(GL_POINTS, 0, vertexSize);
        end();

        std::swap(buffer[0], output);
    }
};

Transformer *ObjectBox::danmakuTransform(Scene *scene) {
    return new DanmakuTransform(scene);
}
