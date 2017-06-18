#include "transform_renderer.hpp"
#include "vertex/trail.hpp"

static Vertex vertex[trailSize + trailHead];

using namespace glm;

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

static constexpr float elapse = Application::elapse * 0.5f;

class TrailTransform : public TransformRenderer<TrailTransformProto> {
protected:
    void begin(GLenum mode) {
        glEnable(GL_RASTERIZER_DISCARD);
        glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, 0, outputBuffer(),
                          trailHead * sizeof(Vertex), trailSize * sizeof(Vertex));
        glBeginTransformFeedback(mode);
    }

    struct {
        Character *character;
        int bone;
        vec3 pos;
        vec3 now;

        vec3 update() {
            return now = character->getGlobal(bone, pos);
        }
    } bind;
public:
    TrailTransform(Scene *scene, Character *character, int bone, vec3 pos)
    : TransformRenderer(scene) {
        bind.character = character;
        bind.bone = bone;
        bind.pos = pos;
    }

    void setup() {
        setupVertices();
        TransformRenderer::setup();

        glUniform1f(uniform[0], elapse);

        bind.update();
    }

    GLuint outputBuffer() {
        return buffer[0];
    }

    void update() {
        std::swap(buffer[0], buffer[1]);

        bindProgram();

        vec3 tmp = bind.now;
        bind.update();

        // if (distance(tmp, bind.now) < 1e-4) {
        //     glUniform1f(uniform[2], 0.0f);
        //     glUniform1f(uniform[4], 0.0f);
        // } else {
        glUniform1f(uniform[2], 1.0f);
        glUniform1f(uniform[4], 1.0f - elapse);
        // }

        glUniform3fv(uniform[3], 1, &tmp[0]);
        glUniform3fv(uniform[1], 1, &bind.now[0]);

        bindBuffer(buffer[1]);

        begin(GL_POINTS);
        glDrawArrays(GL_POINTS, 0, trailSize);
        end();
    }
};

Transformer *ObjectBox::trailTransform(Scene *scene, Character *character,
                                       int bone, vec3 pos) {
    return new TrailTransform(scene, character, bone, pos);
}
