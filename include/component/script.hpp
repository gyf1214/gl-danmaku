#ifndef __COM_SCRIPT
#define __COM_SCRIPT

#include "../core.hpp"

class Script : public virtual Component {
public:
    virtual Renderer *createObjects(void) = 0;
    virtual void run(void) = 0;

    // Factories
    static Script *scene1(void);
};

#endif
