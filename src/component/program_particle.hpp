#ifndef __COM_PROGRAM_PARTICLE
#define __COM_PROGRAM_PARTICLE

#include "../ext.hpp"
#include "../core/program.hpp"
#include "component/shader.hpp"
#include "component/particle.hpp"

template <typename Proto>
class ProgramParticle : public ProgramBase<Proto>, public virtual Particle {
protected:
    virtual void begin(GLenum mode) {
        glEnable(GL_RASTERIZER_DISCARD);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, outputBuffer());
        glBeginTransformFeedback(mode);
    }

    virtual void end() {
        glEndTransformFeedback();
        glDisable(GL_RASTERIZER_DISCARD);
    }
};

#endif
