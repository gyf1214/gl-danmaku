#ifndef __COM_BASIC_MOTION
#define __COM_BASIC_MOTION

#include "component/transform.hpp"

template <typename T>
struct MotionData {
    T now, origin, target;
    float current, end;
    bool playing, looping;

    template <typename U>
    void update(U inter) {
        if (playing) {
            if (current < end) {
                now = inter(origin, target, current / end);
                ++current;
            } else {
                now = target;
                if (looping) {
                    current = 0.0f;
                } else {
                    playing = false;
                }
            }
        }
    }
};

class BasicMotion : public virtual Motion {
    MotionData<vec3> pos;
    MotionData<glm::quat> rot;
public:
    void setup(void);
    void reset(void);
    void update(void);

    vec3 position(void) const;
    glm::quat rotation(void) const;
    glm::mat4 matrix(void) const;

    void teleport(vec3 pos);
    void move(vec3 pos);
    float moveTo(vec3 pos, float speed);

    void lookAt(glm::quat dir);
    float rotateTo(glm::quat dir, float speed);
    float rotateLocal(glm::quat r, float speed);
    float rotateGlobal(glm::quat r, float speed);
};

#endif
