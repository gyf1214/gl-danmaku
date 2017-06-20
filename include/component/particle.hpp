#ifndef __COM_PARTICLE
#define __COM_PARTICLE

#include "../core.hpp"

class Particle : public virtual Component {
public:
    virtual GLuint outputBuffer(void) = 0;
};

#endif
