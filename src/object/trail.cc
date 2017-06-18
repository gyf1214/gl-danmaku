#include "program_renderer.hpp"
#include "layer.hpp"
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
    Layer *layer;
public:
    Trail(Scene *scene, Transformer *transform)
        : ProgramRenderer(scene), transform(transform) {}

    void setup() {
        ProgramRenderer::setup();

        glUniform1i(uniform[2], 0);
        texture0 = Texture::trail();

        layer = Layer::temp();
    }

    void render() {
        if (scene->pass() > 0) return;

        layer->select();
        Layer::clear();

        bindProgram();
        glEnable(GL_PROGRAM_POINT_SIZE);
        glEnable(GL_BLEND);
        glBlendEquationSeparate(GL_MAX, GL_MAX);
        glDepthFunc(GL_ALWAYS);

        glUniformMatrix4fv(uniform[0], 1, GL_FALSE, &scene -> vMat()[0][0]);
        glUniformMatrix4fv(uniform[1], 1, GL_FALSE, &scene -> pMat()[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);

        bindBuffer(transform->outputBuffer());

        glDrawArrays(GL_POINTS, trailHead, trailSize);

        glDepthFunc(GL_LEQUAL);
        glDisable(GL_BLEND);
        glDisable(GL_PROGRAM_POINT_SIZE);

        Layer::detach();
        layer->render();
    }
};

Renderer *ObjectBox::trail(Scene *scene, Transformer *transform) {
    return create<Trail>(scene, transform);
}
