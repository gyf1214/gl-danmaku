#include <GL/glew.h>
#include <glm/glm.hpp>
#include <algorithm>
#include "transform_renderer.hpp"
#include "vertex.hpp"
#include "object_box.hpp"
#include "shader.hpp"
#include "util.hpp"

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
};

protoUnifom = {};

static void setupVertices() {
    LOG << "setup vertices";

    for (int i = 0; i < vertexSize; ++i) {
        float angle = (float)i / (float)vertexSize * M_PI * 2.0f;

        glm::vec3 x = glm::vec3(cos(angle), sin(angle), 0.0f);
        // glm::vec3 rnd = normalize(glm::vec3(randomNeg(), randomNeg(), randomNeg()));
        glm::vec3 rnd2 = normalize(glm::vec3(randomNeg(), randomNeg(), randomNeg()));

        vertexData[i].time = glm::vec4(0.0f, 0.0f, 1.0f, INFINITY);
        vertexData[i].position = x;
        vertexData[i].velocity = rnd2 * 0.2f;
        vertexData[i].acceleration = x;
    }

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
