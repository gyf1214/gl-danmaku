#ifndef __COM_LAYER
#define __COM_LAYER

#include "../core.hpp"

class Layer : public virtual BaseObject {
public:
    void select(void);
    void attach(void);

    static void detach(void);
    static void clear(float alpha = 0.0f, vec3 color = vec3(0.0f));
    static void release(void);
    static Layer *temp(void);
};

#endif
