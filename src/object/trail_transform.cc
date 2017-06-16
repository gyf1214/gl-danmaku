#include "transform_renderer.hpp"
#include "vertex/trail.hpp"

static Vertex vertex[trailSize + 1];

proto(TrailTransform, Shader::trailTransform);

protoBuffer = {
    { sizeof(vertex), vertex },
    { sizeof(vertex), vertex }
};

protoAttrib = {
    { "position0", Offset(Vertex, position[0]), 3, sizeof(Vertex) },
    { "alpha0"   , Offset(Vertex, alpha)      , 1, sizeof(Vertex) }
};

protoUnifom = { "position1", "alpha1", "elapse" };

static void setupVertices() {
    memset(vertex, 0, sizeof(vertex));
    vertex[0].alpha = -1.0f;
}

class TrailTransform : public TransformRenderer<Proto> {
protected:
    void begin(GLenum mode) {
        glEnable(GL_RASTERIZER_DISCARD);
        glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, 0, outputBuffer(),
                          sizeof(Vertex), trailSize * sizeof(Vertex));
        glBeginTransformFeedback(mode);
    }
public:
    TrailTransform(Scene *scene) : TransformRenderer(scene) {}

    void setup() {
        setupVertices();
        TransformRenderer::setup();

        glUniform1f(uniform[2], Application::elapse);
    }

    GLuint outputBuffer() {
        return buffer[0];
    }

    void update() {
        std::swap(buffer[0], buffer[1]);

        bindProgram();

        static vec3 now(0.0f, 0.0f, 10.0f);
        glUniform3fv(uniform[0], 1, &now[0]);
        glUniform1f(uniform[1], 1.0f);
        now.z += 0.1f;

        bindBuffer(buffer[1]);

        begin(GL_POINTS);
        glDrawArrays(GL_POINTS, 0, trailSize);
        end();
    }
};

Transformer *ObjectBox::trailTransform(Scene *scene) {
    return new TrailTransform(scene);
}
