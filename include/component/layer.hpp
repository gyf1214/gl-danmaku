#ifndef __COM_LAYER
#define __COM_LAYER

#include "../core.hpp"

class Layer : public virtual BaseObject {
public:
    virtual void select(void) = 0;
    virtual void attach(void) = 0;

    static void detach(void);
    static void clear(float alpha = 0.0f, const vec3 &color = vec3(0.0f));
    static void release(void);
    static Layer *temp(void);
};

#endif
