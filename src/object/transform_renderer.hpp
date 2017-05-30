#ifndef __TRANFORM
#define __TRANFORM

#include "program_renderer.hpp"

#define transProto(name, program) protoOpen(name, program)\
    static const BufferProto Output;\
} Proto

#define protoOutput const BufferProto Proto::Output

template <typename Proto>
class TransformRenderer : public ProgramBase<Proto>, public Transformer {
protected:
    GLuint output;

    TransformRenderer(Scene *scene) : Transformer(scene) {}

    void setupBuffers() {
        ProgramBase<Proto>::setupBuffers();

        const auto &proto = Proto::Output;
        glGenBuffers(1, &output);
        LOG << "set output buffer: " << output;

        glBindBuffer(GL_ARRAY_BUFFER, output);
        glBufferData(GL_ARRAY_BUFFER, proto.size, proto.data, proto.usage);
    }

    void begin(GLenum mode) {
        glEnable(GL_RASTERIZER_DISCARD);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, output);
        glBeginTransformFeedback(mode);
    }

    void end() {
        glEndTransformFeedback();
        glDisable(GL_RASTERIZER_DISCARD);
    }
public:
    void setup() {
        this -> setupAll();
    }

    void reset() {
        this -> resetAll();

        glDeleteBuffers(1, &output);
    }

    GLuint outputBuffer() {
        return output;
    }
};

#endif
