#include "layer_program.hpp"
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
    LayerProgram *program;
    float size;
    vec3 color;
public:
    TrailObject(Particle *particle, Camera *camera, float size, vec3 color)
        : camera(camera), particle(particle), size(size), color(color) {}

    void setup() {
        layer = Layer::temp();
        program = Box::create<LayerProgram>(Shader::layerTransparent());
        program->setup();
        glUseProgram(program->program);

        GLuint color0 = glGetUniformLocation(program->program, "color0");
        GLuint depth0 = glGetUniformLocation(program->program, "depth0");
        GLuint depth1 = glGetUniformLocation(program->program, "depth1");

        glUniform1i(color0, 5);
        glUniform1i(depth0, 6);
        glUniform1i(depth1, 7);

        ProgramBase::setup();

        glUniform1i(uniform[2], 0);
    }

    void reset() {
        ProgramBase::reset();

        Box::release(program);
    }

    void render() {
        int fbo;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);

        layer->select();
        Layer::clear(0.0f);

        bindProgram();
        glDepthFunc(GL_ALWAYS);
        glEnable(GL_PROGRAM_POINT_SIZE);
        glEnable(GL_BLEND);
        glBlendEquation(GL_MAX);

        glUniform1f(uniform[3], size * Application::bufferHeight /
                                (2.0f * tan(camera->fovy() / 2.0f)));
        glUniformMatrix4fv(uniform[0], 1, GL_FALSE, &camera->vMat()[0][0]);
        glUniformMatrix4fv(uniform[1], 1, GL_FALSE, &camera->pMat()[0][0]);
        glUniform3fv(uniform[4], 1, &color[0]);

        bindBuffer(particle->outputBuffer());
        bindTextures();

        glDrawArrays(GL_POINTS, particle->offset(), particle->size());

        glDisable(GL_BLEND);
        glDisable(GL_PROGRAM_POINT_SIZE);
        glDepthFunc(GL_LEQUAL);

        // Layer::detach();
        // layer->blit();

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        program->bind(layer);
        program->render();

        // Layer::detach();
        // layer->attach();
    }
};

Object *ObjectBox::trail(Particle *particle, Camera *camera, float size, vec3 color) {
    return Box::create<TrailObject>(particle, camera, size, color);
}
