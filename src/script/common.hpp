#ifndef __COM_EXT_SCRIPT
#define __COM_EXT_SCRIPT

#include "../ext.hpp"
#include "component.hpp"

class BasicScript : public virtual Script {
    static void doFiber(void *self);
protected:
    std::vector<Component *> coms;
    Fiber *fiber;
    float frame, nextEvent;
    bool waiting;

    Camera *camera;
    LightManager *light;
    Renderer *root;

    void pushBase(Component *c);
    void pushBase(Object *c);

    template <typename U>
    U *push(U *u) { pushBase(u); return u; }

    void await();
    void await(float x);
    void awaitUntil(float x);
public:
    void setup();
    void reset();
    void update();
    void createObjects(Renderer *root);
};

#endif
