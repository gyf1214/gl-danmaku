#ifndef __COM_CHARACTER
#define __COM_CHARACTER

#include "transform.hpp"

class Keyframe : public virtual Component {
public:
    virtual float play(float start, float end, float speed = 1.0f) = 0;
    virtual float playTo(float end, float speed = 1.0f) = 0;
    virtual void loop(float start, float end, float speed = 1.0f) = 0;
    virtual void loopTo(float end, float speed = 1.0f) = 0;
    virtual void fix(float start) = 0;
    virtual void pause(void) = 0;
    virtual float frame(void) const = 0;
};

class Character : public virtual Motion, public virtual Keyframe {
public:
    virtual Translate *bindPoint(int bone, vec3 origin) const = 0;
    virtual const std::vector<glm::mat4> &skins(void) const = 0;
    virtual const std::vector<float> &face(int morph) const = 0;
};

#endif
