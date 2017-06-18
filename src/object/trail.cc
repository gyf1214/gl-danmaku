#include "program_renderer.hpp"
#include "layer.hpp"
#include "vertex/trail.hpp"

proto(Trail, Shader::trail);

protoBuffer(Trail) = {};

protoAttrib(Trail) = {
    { "position", Offset(Vertex, position[0]), 3, sizeof(Vertex) },
    { "alpha"   , Offset(Vertex, alpha)      , 1, sizeof(Vertex) }
};

protoUnifom(Trail) = {
    "vMat", "pMat", "texture0", "size", "color"
};

class Trail : public ProgramRenderer<TrailProto> {
    Transformer *transform;
    GLuint texture0;
    Layer *layer;
    float size;
    vec3 color;
public:
    Trail(Scene *scene, Transformer *transform, float size, vec3 color)
        : ProgramRenderer(scene), transform(transform),
          size(size), color(color) {}

    void setup() {
        ProgramRenderer::setup();

        glUniform3fv(uniform[4], 1, &color[0]);

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

        glUniform1f(uniform[3], size * Application::bufferHeight /
                                (2.0f * tan(scene->fovy() / 2.0f)));
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

Renderer *ObjectBox::trail(Scene *scene, Transformer *transform,
                           float size, vec3 color) {
    return create<Trail>(scene, transform, size, color);
}
