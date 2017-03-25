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
    srand(time(NULL));

    constexpr int roundSize = vertexSize / vertexDirections;
    glm::vec3 zero = glm::vec3(0.0f, 0.0f, 0.0f);
    int cnt = 0;
    // for (int k = 0; k < vertexDirections; ++k) {
    //     glm::vec3 rnd2 = vertexDir[k];
    //     glm::vec3 zero = glm::vec3(0.0f, 0.0f, 0.0f);
    //
    //     for (int i = 0; i < roundSize; ++i) {
    //         float t = (float)i * 0.1f;
    //
    //         vertexData[cnt].time = glm::vec4(t, INFINITY, t, INFINITY);
    //         vertexData[cnt].position = rnd2 * 0.2f;
    //         vertexData[cnt].velocity = rnd2 * 10.0f;
    //         vertexData[cnt++].acceleration = zero;
    //     }
    // }
    // for (int i = 0; i < vertexSize; ++i) {
    //     float t = (float)i / (float)vertexSize;
    //     float angle = t * 2.0f * M_PI;
    //
    //     glm::vec3 rnd = normalize(glm::vec3(randomNeg(), randomNeg(), randomNeg()));
    //     glm::vec3 dir = glm::vec3(cos(angle), sin(angle), 0.0f);
    //
    //     vertexData[i].time = glm::vec4(t, INFINITY, 2.0f, INFINITY);
    //     vertexData[i].position = dir;
    //     vertexData[i].velocity = rnd * 0.2f;
    //     vertexData[i].acceleration = dir;
    // }
    for (int k = 0; k < vertexDirections; ++k) {
        float angle = (float)k / (float)vertexDirections * 2.0f * M_PI;
        for (int i = 0; i < roundSize; ++i) {
            float t = (float)i * 0.1f;
            float aa = angle + M_PI * cos(t);
            glm::vec3 dir = glm::vec3(cos(aa), sin(aa), 0.0f);
            vertexData[cnt].time = glm::vec4(t, INFINITY, t, INFINITY);
            vertexData[cnt].position = dir * 0.2f;
            vertexData[cnt].velocity = dir * 5.0f;
            vertexData[cnt++].acceleration = zero;
        }
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
