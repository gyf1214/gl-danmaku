#ifndef __CORE_OBJECT
#define __CORE_OBJECT

#include "../common.hpp"

class BaseObject {
public:
    virtual ~BaseObject();
    virtual void setup(void) = 0;
    virtual void reset(void) = 0;
};

class Object : public virtual BaseObject {
public:
    virtual void render(void) = 0;
};

class Renderer : public virtual Object {
public:
    virtual void push(Object *o) = 0;
};

class Component : public virtual BaseObject {
public:
    virtual void update(void) = 0;
};

class Scene : public virtual Object, public virtual Component {};

#endif
