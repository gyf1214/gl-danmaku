#ifndef __COM_BASIC_MOTION
#define __COM_BASIC_MOTION

#include "component/transform.hpp"

template <typename T>
struct InterFunc {
    typedef T (*ref)(const T &, const T &, float);
    typedef T (*val)(T, T, float);
};

typedef InterFunc<vec3>::ref InterVec3;
typedef InterFunc<float>::val InterFloat;
typedef InterFunc<glm::quat>::ref InterQuat;

inline float linearStep(float a, float b, float x) {
    return (x - a) / (b - a);
}

template <typename T>
struct MotionData {
    T now, origin, target;
    float current, end;
    bool playing, looping;

    template <typename U, typename V>
    void update(U inter, V inter2) {
        if (playing) {
            if (current < end) {
                now = inter(origin, target, inter2(0.0f, end, current));
                current += Application::elapse();
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

    template <typename U>
    void update(U inter) { update(inter, (InterFloat)linearStep); }
};

class BasicMotion : public virtual Motion {
protected:
    MotionData<vec3> pos;
    MotionData<glm::quat> rot;
public:
    BasicMotion();

    void setup(void);
    void reset(void);
    void update(void);

    vec3 position(void) const;
    glm::quat rotation(void) const;
    glm::mat4 matrix(void) const;

    void teleport(vec3 pos);
    void move(vec3 pos);
    float moveTo(vec3 pos, float time);
    float waypoint(vec3 pos, float time);
    void autoRot(bool e);

    void lookAt(glm::quat dir);
    float rotateTo(glm::quat dir, float time);
    float rotateLocal(glm::quat r, float time);
    float rotateGlobal(glm::quat r, float time);
};

#endif
