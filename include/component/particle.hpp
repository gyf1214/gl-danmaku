#ifndef __COM_PARTICLE
#define __COM_PARTICLE

#include "../core.hpp"
#include "provider.hpp"
#include "transform.hpp"

class Particle : public virtual Component {
public:
    virtual GLuint outputBuffer(void) const = 0;
    virtual int offset(void) const = 0;
    virtual int size(void) const = 0;
    virtual void enable(bool e) = 0;

    // Factories
    static Particle *danmaku(Provider *provider);
    static Particle *trail(Translate *translate);
};

#endif
