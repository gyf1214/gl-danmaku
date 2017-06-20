#ifndef __COM_CHARACTER
#define __COM_CHARACTER

#include "transform.hpp"

class Keyframe : public virtual Component {
public:
    virtual void resume(void) = 0;
    virtual void pause(void) = 0;
    virtual void play(float start, float speed = 1.0f) = 0;
    virtual float playRange(float start, float end, float speed = 1.0f) = 0;
    virtual float playTo(float end, float speed = 1.0f) = 0;
    virtual void loop(float start, float end, float speed = 1.0f) = 0;
    virtual void loopTo(float end, float speed = 1.0f) = 0;
    virtual void fix(float start) = 0;
    virtual float current(void) = 0;
};

class Model : public virtual Component {
public:
    virtual GLuint texture(int index) = 0;
    virtual GLuint morphTexture(void) = 0;
};

class Character : public virtual Motion, public virtual Keyframe {
public:
    virtual Transform *bindPoint(int bone, vec3 origin) = 0;
};

#endif
