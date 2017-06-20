#include "ext.hpp"
#include "../core/program.hpp"
#include "component/particle.hpp"
#include "vertex/danmaku.hpp"

proto(Danmaku, Shader::danmaku);

protoBuffer(Danmaku) = {};

protoAttrib(Danmaku) = {
    { "time"    , Offset(Vertex, time[0])    , 4, sizeof(Vertex) },
    { "position", Offset(Vertex, position[0]), 3, sizeof(Vertex) },
    { "velocity", Offset(Vertex, velocity[0]), 3, sizeof(Vertex) },
    { "uvIndex" , Offset(Vertex, uvIndex[0]) , 4, sizeof(Vertex) },
};

protoUniform(Danmaku) = { "vMat", "pMat", "texture0" };

protoTexture(Danmaku) = { Texture::etama };

class Danmaku : public ProgramBase<DanmakuProto>, public virtual Object {
    Particle *particle;
    Camera *camera;
public:
    Danmaku(Particle *particle, Camera *camera)
        : particle(particle), camera(camera) {}

    void setup() {
        ProgramBase::setup();

        glUniform1i(uniform[2], 0);
    }

    void render() {
        bindProgram();

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);
        glDepthMask(GL_FALSE);

        glUniformMatrix4fv(uniform[0], 1, GL_FALSE, &camera->vMat()[0][0]);
        glUniformMatrix4fv(uniform[1], 1, GL_FALSE, &camera->pMat()[0][0]);

        bindBuffer(particle->outputBuffer());
        bindTextures();

        glDrawArrays(GL_POINTS, 0, vertexSize);

        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
    }
};
