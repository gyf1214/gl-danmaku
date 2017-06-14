#ifndef __TRANFORM
#define __TRANFORM

#include "program_renderer.hpp"

template <typename Proto>
class TransformRenderer : public ProgramBase<Proto>, public Transformer {
protected:
    TransformRenderer(Scene *scene) : Transformer(scene) {}

    virtual void begin(GLenum mode) {
        glEnable(GL_RASTERIZER_DISCARD);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, outputBuffer());
        glBeginTransformFeedback(mode);
    }

    virtual void end() {
        glEndTransformFeedback();
        glDisable(GL_RASTERIZER_DISCARD);
    }
public:
    void setup() {
        this -> setupAll();
    }

    void reset() {
        this -> resetAll();
    }
};

#endif
