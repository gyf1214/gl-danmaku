#include "program_renderer.hpp"
#include "vertex/trail.hpp"

proto(Trail, Shader::trail);

protoBuffer(Trail) = {};

protoAttrib(Trail) = {
    { "position", Offset(Vertex, position[0]), 3, sizeof(Vertex) },
    { "alpha"   , Offset(Vertex, alpha)      , 1, sizeof(Vertex) }
};

protoUnifom(Trail) = { "vMat", "pMat", "texture0" };

class Trail : public ProgramRenderer<TrailProto> {
    Transformer *transform;
    GLuint texture0;
public:
    Trail(Scene *scene, Transformer *transform)
        : ProgramRenderer(scene), transform(transform) {}

    void setup() {
        ProgramRenderer::setup();

        glUniform1i(uniform[2], 0);
        texture0 = Texture::trail();
    }

    void render() {
        if (scene->pass() > 0) return;

        // glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        // glClear(GL_COLOR_BUFFER_BIT);

        bindProgram();
        glEnable(GL_BLEND);
        glEnable(GL_PROGRAM_POINT_SIZE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // glBlendFunc(GL_ONE, GL_ZERO);
        // glBlendEquationSeparate(GL_FUNC_ADD, GL_MAX);

        glDepthMask(GL_FALSE);

        glUniformMatrix4fv(uniform[0], 1, GL_FALSE, &scene -> vMat()[0][0]);
        glUniformMatrix4fv(uniform[1], 1, GL_FALSE, &scene -> pMat()[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);

        bindBuffer(transform->outputBuffer());

        glDrawArrays(GL_POINTS, trailHead, trailSize);

        glDepthMask(GL_TRUE);
        glDisable(GL_PROGRAM_POINT_SIZE);
        glDisable(GL_BLEND);
    }
};

Renderer *ObjectBox::trail(Scene *scene, Transformer *transform) {
    return create<Trail>(scene, transform);
}
