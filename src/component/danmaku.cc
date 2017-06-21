#include "component/provider.hpp"
#include "vertex/danmaku.hpp"
#include "program_particle.hpp"

proto(DanmakuTransform, Shader::danmakuTransform);

protoBuffer(DanmakuTransform) = {
    { 0, NULL },
    { 0, NULL }
};

protoAttrib(DanmakuTransform) = {
    { "time0"        , Offset(Vertex, time[0])        , 4, sizeof(Vertex) },
    { "position0"    , Offset(Vertex, position[0])    , 3, sizeof(Vertex) },
    { "velocity0"    , Offset(Vertex, velocity[0])    , 3, sizeof(Vertex) },
    { "acceleration0", Offset(Vertex, acceleration[0]), 4, sizeof(Vertex) },
    { "uvIndex0"     , Offset(Vertex, uvIndex[0])     , 4, sizeof(Vertex) },
};

protoUniform(DanmakuTransform) = {};
protoTexture(DanmakuTransform) = {};

class Danmaku : public ProgramParticle<DanmakuTransformProto> {
    Provider *provider;
    int count;
public:
    Danmaku(Provider *provider) : provider(provider) {}

    virtual void setupBuffers() {
        if (!BufferSize) return;
        glGenBuffers(BufferSize, buffer);
        LOG << "generate " << BufferSize << " buffers";
        size_t size = provider->size();
        count = size / sizeof(Vertex);
        glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
        glBufferData(GL_ARRAY_BUFFER, size, provider->data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
        glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
    }

    void update() {
        std::swap(buffer[0], buffer[1]);
        bindProgram();
        bindBuffer(buffer[1]);

        begin(GL_POINTS);
        glDrawArrays(GL_POINTS, 0, count);
        end();
    }

    GLuint outputBuffer() const { return buffer[0]; }
    int offset() const { return 0; }
    int size() const { return count * sizeof(Vertex); }
};

Particle *Particle::danmaku(Provider *provider) {
    return Box::create<Danmaku>(provider);
}
