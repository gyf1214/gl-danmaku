#ifndef __COM_TRANSFORM
#define __COM_TRANSFORM

#include "../core.hpp"

class Translate : public virtual Component {
public:
    virtual vec3 position(void) const = 0;
};

class Transform : public virtual Translate {
public:
    virtual glm::quat rotation(void) const = 0;
    virtual glm::mat4 matrix(void) const = 0;
};

class Motion : public virtual Transform {
public:
    virtual void teleport(vec3 pos) = 0;
    virtual void move(vec3 pos) = 0;
    virtual float moveTo(vec3 pos, float time) = 0;
    virtual float waypoint(vec3 pos, float time) = 0;

    virtual void lookAt(glm::quat dir) = 0;
    virtual float rotateTo(glm::quat dir, float time) = 0;
    virtual float rotateLocal(glm::quat r, float time) = 0;
    virtual float rotateGlobal(glm::quat r, float time) = 0;

    virtual void autoRot(bool e) = 0;

    void teleport(float x, float y, float z) { teleport(vec3(x, y, z)); }
    void move(float x, float y, float z) { move(vec3(x, y, z)); }
    float waypoint(float x, float y, float z, float time) {
        return waypoint(vec3(x, y, z), time);
    }
    float moveTo(float x, float y, float z, float time) {
        return moveTo(vec3(x, y, z), time);
    }

    static Motion *spline();
};

#endif
