#include "../ext.hpp"
#include "basic_motion.hpp"

using namespace glm;

BasicMotion::BasicMotion() {
    pos.now = vec3(0.0f);
    pos.playing = pos.looping = false;
    rot.now = quat();
    rot.playing = rot.playing = false;
}

void BasicMotion::setup() {}
void BasicMotion::reset() {}

void BasicMotion::update() {
    pos.update((InterVec3)mix, (InterFloat)smoothstep);
    rot.update((InterQuat)slerp);
}

vec3 BasicMotion::position() const { return pos.now; }

quat BasicMotion::rotation() const { return rot.now; }

mat4 BasicMotion::matrix() const {
    return translate(pos.now) * mat4_cast(rot.now);
}

void BasicMotion::teleport(vec3 p) { pos.now = p; }

void BasicMotion::move(vec3 p) { pos.now = p; }

float BasicMotion::moveTo(vec3 p, float time) {
    vec3 cur = pos.now;
    pos.end = time;
    pos.current = 0.0f;
    pos.origin = cur;
    pos.target = p;
    pos.playing = true;
    return pos.end;
}

void BasicMotion::lookAt(quat dir) { rot.now = dir; }

float BasicMotion::rotateTo(quat dir, float time) {
    rot.end = time;
    rot.current = 0.0f;
    rot.origin = rot.now;
    rot.target = dir;
    rot.playing = true;
    return rot.end;
}

float BasicMotion::rotateLocal(quat r, float speed) {
    return rotateTo(rot.now * r, speed);
}

float BasicMotion::rotateGlobal(quat r, float speed) {
    return rotateTo(r * rot.now, speed);
}

float BasicMotion::waypoint(vec3 pos, float time) {
    FAIL << "not implemented!";
    return 0.0f;
}
