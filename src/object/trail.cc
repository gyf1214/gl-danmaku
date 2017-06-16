#include "program_renderer.hpp"
#include "vertex/trail.hpp"

proto(Trail, Shader::trail);

protoBuffer = {};

protoAttrib = {
    { "position", Offset(Vertex, position[0]), 3, sizeof(Vertex) },
    { "alpha"   , Offset(Vertex, alpha)      , 1, sizeof(Vertex) }
};

protoUnifom = { "vMat", "pMat" };

class Trail : public ProgramRenderer<Proto> {
    Transformer *transform;
public:
    Trail(Scene *scene, Transformer *transform)
        : ProgramRenderer(scene), transform(transform) {}

    void render() {
        if (scene->pass() > 0) return;

        bindProgram();
        glEnable(GL_BLEND);
        glEnable(GL_PROGRAM_POINT_SIZE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);

        glUniformMatrix4fv(uniform[0], 1, GL_FALSE, &scene -> vMat()[0][0]);
        glUniformMatrix4fv(uniform[1], 1, GL_FALSE, &scene -> pMat()[0][0]);

        bindBuffer(transform->outputBuffer());

        glDrawArrays(GL_POINTS, 1, trailSize);

        glDepthMask(GL_TRUE);
        glDisable(GL_PROGRAM_POINT_SIZE);
        glDisable(GL_BLEND);
    }
};

Renderer *ObjectBox::trail(Scene *scene, Transformer *transform) {
    return create<Trail>(scene, transform);
}
