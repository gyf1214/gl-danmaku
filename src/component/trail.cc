#include "program_particle.hpp"
#include "component/transform.hpp"
#include "vertex/trail.hpp"

using namespace glm;

static constexpr int trailSize = 10000;
static constexpr int trailHead = 5;

static Vertex vertex[trailSize + trailHead];
static constexpr float elapse = Application::constantElapse * 0.5f;

proto(TrailTransform, Shader::trailTransform);

protoBuffer(TrailTransform) = {
    { sizeof(vertex), vertex },
    { sizeof(vertex), vertex }
};

protoAttrib(TrailTransform) = {
    { "position0", Offset(Vertex, position[0]), 3, sizeof(Vertex) },
    { "alpha0"   , Offset(Vertex, alpha)      , 1, sizeof(Vertex) }
};

protoUniform(TrailTransform) = {
    "elapse", "position1", "alpha1",
    "position2", "alpha2"
};

protoTexture(TrailTransform) = {};

static void setupVertices() {
    memset(vertex, 0, sizeof(vertex));
    for (int i = 0; i < trailHead; ++i) {
        vertex[i].alpha = (float)i / (float)trailHead - 1.0f;
    }

}

class Trail : public ProgramParticle<TrailTransformProto> {
    Translate *bind;
    vec3 lastPos;
protected:
    void begin(GLenum mode) {
        glEnable(GL_RASTERIZER_DISCARD);
        glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, 0, outputBuffer(),
                          trailHead * sizeof(Vertex), trailSize * sizeof(Vertex));
        glBeginTransformFeedback(mode);
    }
public:
    Trail(Translate *bind) : bind(bind) {}

    void setup() {
        setupVertices();
        ProgramParticle::setup();

        glUniform1f(uniform[0], elapse);
        lastPos = bind->position();
    }

    void update() {
        std::swap(buffer[0], buffer[1]);

        bindProgram();

        glUniform1f(uniform[2], 1.0f);
        glUniform1f(uniform[4], 1.0f - elapse);

        glUniform3fv(uniform[3], 1, &lastPos[0]);
        glUniform3fv(uniform[1], 1, &bind->position()[0]);
        lastPos = bind->position();

        bindBuffer(buffer[1]);

        begin(GL_POINTS);
        glDrawArrays(GL_POINTS, 0, trailSize);
        end();
    }

    GLuint outputBuffer() const { return buffer[0]; }
    int offset() const { return trailHead; }
    int size() const { return trailSize; }
};

Particle *Particle::trail(Translate *translate) {
    return Box::create<Trail>(translate);
}
