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
    pos.update((vec3 (*)(const vec3 &, const vec3 &, float))mix);
    rot.update((quat (*)(const quat &, const quat &, float))slerp);
}

vec3 BasicMotion::position() const { return pos.now; }

quat BasicMotion::rotation() const { return rot.now; }

mat4 BasicMotion::matrix() const {
    return translate(pos.now) * mat4_cast(rot.now);
}

void BasicMotion::teleport(vec3 p) { pos.now = p; }

void BasicMotion::move(vec3 p) { pos.now = p; }

float BasicMotion::moveTo(vec3 p, float speed) {
    vec3 cur = pos.now;
    pos.end = distance(p, cur) / speed / Application::elapse;
    pos.current = 0.0f;
    pos.origin = cur;
    pos.target = p;
    pos.playing = true;
    return pos.end * Application::elapse;
}

void BasicMotion::lookAt(quat dir) { rot.now = dir; }

float BasicMotion::rotateTo(quat dir, float speed) {
    float dis = acos(dot(dir, rot.now));
    rot.end = dis / speed / Application::elapse;
    rot.current = 0.0f;
    rot.origin = rot.now;
    rot.target = dir;
    rot.playing = true;
    return rot.end * Application::elapse;
}

float BasicMotion::rotateLocal(quat r, float speed) {
    return rotateTo(rot.now * r, speed);
}

float BasicMotion::rotateGlobal(quat r, float speed) {
    return rotateTo(r * rot.now, speed);
}
