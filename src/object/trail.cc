#include "ext.hpp"
#include "vertex/trail.hpp"

proto(Trail, Shader::trail);

protoBuffer(Trail) = {};

protoAttrib(Trail) = {
    { "position", Offset(Vertex, position[0]), 3, sizeof(Vertex) },
    { "alpha"   , Offset(Vertex, alpha)      , 1, sizeof(Vertex) }
};

protoUniform(Trail) = {
    "vMat", "pMat", "texture0", "size", "color"
};

protoTexture(Trail) = { Texture::trail };

class TrailObject : public ProgramBase<TrailProto>, public virtual Object {
    Camera *camera;
    Particle *particle;
    Layer *layer;
    float size;
    vec3 color;
public:
    TrailObject(Particle *particle, Camera *camera, float size, vec3 color)
        : camera(camera), particle(particle), size(size), color(color) {}

    void setup() {
        layer = Layer::temp();
        ProgramBase::setup();

        glUniform3fv(uniform[4], 1, &color[0]);
        glUniform1i(uniform[2], 0);
    }

    void render() {
        int fbo;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);

        layer->select();
        Layer::clear(0.0f);

        bindProgram();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_ALWAYS);
        glEnable(GL_PROGRAM_POINT_SIZE);
        glEnable(GL_BLEND);
        glBlendEquation(GL_MAX);

        glUniform1f(uniform[3], size * Application::bufferHeight /
                                (2.0f * tan(camera->fovy() / 2.0f)));
        glUniformMatrix4fv(uniform[0], 1, GL_FALSE, &camera->vMat()[0][0]);
        glUniformMatrix4fv(uniform[1], 1, GL_FALSE, &camera->pMat()[0][0]);

        bindBuffer(particle->outputBuffer());
        bindTextures();

        glDrawArrays(GL_POINTS, particle->offset(), particle->size());

        glDisable(GL_BLEND);
        glDisable(GL_PROGRAM_POINT_SIZE);
        glDisable(GL_DEPTH_TEST);

        // Layer::detach();
        // layer->attach();
    }
};

Object *ObjectBox::trail(Particle *particle, Camera *camera, float size, vec3 color) {
    return Box::create<TrailObject>(particle, camera, size, color);
}
