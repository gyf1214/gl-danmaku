#include "ext.hpp"
#include "vertex/danmaku.hpp"

proto(Danmaku, Shader::danmaku);

protoBuffer(Danmaku) = {};

protoAttrib(Danmaku) = {
    { "time"    , Offset(Vertex, time[0])    , 4, sizeof(Vertex) },
    { "position", Offset(Vertex, position[0]), 3, sizeof(Vertex) },
    { "velocity", Offset(Vertex, velocity[0]), 3, sizeof(Vertex) },
    { "uvIndex" , Offset(Vertex, uvIndex[0]) , 4, sizeof(Vertex) },
};

protoUniform(Danmaku) = {
    "vMat", "pMat", "texture0",
    "color0", "depth0", "depth1", "size"
    // "depth0", "depth1", "size"
};

protoTexture(Danmaku) = { Texture::etama };

class DanmakuObject : public ProgramBase<DanmakuProto>, public virtual Object {
    Particle *particle;
    Camera *camera;
public:
    DanmakuObject(Particle *particle, Camera *camera)
        : particle(particle), camera(camera) {}

    void setup() {
        ProgramBase::setup();

        glUniform1i(uniform[2], 0);
        glUniform1i(uniform[3], 5);
        glUniform1i(uniform[4], 6);
        glUniform1i(uniform[5], 7);

        float size[] = {
            (float)Application::bufferWidth,
            (float)Application::bufferHeight
        };
        glUniform2fv(uniform[6], 1, size);
    }

    void render() {
        bindProgram();

        // glEnable(GL_DEPTH_TEST);
        // glDepthFunc(GL_LEQUAL);
        // glEnable(GL_BLEND);
        // glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        // glBlendEquation(GL_FUNC_ADD);
        // glDepthMask(GL_FALSE);

        glUniformMatrix4fv(uniform[0], 1, GL_FALSE, &camera->vMat()[0][0]);
        glUniformMatrix4fv(uniform[1], 1, GL_FALSE, &camera->pMat()[0][0]);

        bindBuffer(particle->outputBuffer());
        bindTextures();

        glDrawArrays(GL_POINTS, particle->offset(), particle->size());

        // glDepthMask(GL_TRUE);
        // glDisable(GL_BLEND);
        // glDisable(GL_DEPTH_TEST);
    }
};

Object *ObjectBox::danmaku(Particle *particle, Camera *camera) {
    return Box::create<DanmakuObject>(particle, camera);
}
