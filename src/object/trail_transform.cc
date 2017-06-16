#include "transform_renderer.hpp"
#include "vertex/trail.hpp"

static Vertex vertex[trailSize + trailHead];

proto(TrailTransform, Shader::trailTransform);

protoBuffer(TrailTransform) = {
    { sizeof(vertex), vertex },
    { sizeof(vertex), vertex }
};

protoAttrib(TrailTransform) = {
    { "position0", Offset(Vertex, position[0]), 3, sizeof(Vertex) },
    { "alpha0"   , Offset(Vertex, alpha)      , 1, sizeof(Vertex) }
};

protoUnifom(TrailTransform) = {
    "elapse", "position1", "alpha1",
    "position2", "alpha2"
};

static void setupVertices() {
    memset(vertex, 0, sizeof(vertex));
    for (int i = 0; i < trailHead; ++i) {
        vertex[i].alpha = (float)i / (float)trailHead - 1.0f;
    }

}

class TrailTransform : public TransformRenderer<TrailTransformProto> {
protected:
    void begin(GLenum mode) {
        glEnable(GL_RASTERIZER_DISCARD);
        glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, 0, outputBuffer(),
                          trailHead * sizeof(Vertex), trailSize * sizeof(Vertex));
        glBeginTransformFeedback(mode);
    }
public:
    TrailTransform(Scene *scene) : TransformRenderer(scene) {}

    void setup() {
        setupVertices();
        TransformRenderer::setup();

        glUniform1f(uniform[0], Application::elapse);
        glUniform1f(uniform[2], 1.0f);
        glUniform1f(uniform[4], 1.0f - Application::elapse);
    }

    GLuint outputBuffer() {
        return buffer[0];
    }

    void update() {
        std::swap(buffer[0], buffer[1]);

        bindProgram();

        static vec3 now(0.0f, 0.0f, 10.0f);
        glUniform3fv(uniform[3], 1, &now[0]);
        now.z += 0.1f;
        glUniform3fv(uniform[1], 1, &now[0]);

        bindBuffer(buffer[1]);

        begin(GL_POINTS);
        glDrawArrays(GL_POINTS, 0, trailSize);
        end();
    }
};

Transformer *ObjectBox::trailTransform(Scene *scene) {
    return new TrailTransform(scene);
}
