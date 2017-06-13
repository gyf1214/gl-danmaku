#include "ext.hpp"

using namespace glm;

void Character::updateFrame() {
    if (frame.play) {
        frame.current += frame.speed;
        if (frame.range && frame.current >= frame.end) {
            if (frame.loop) {
                frame.current -= frame.end - frame.begin;
            } else {
                frame.current = frame.end;
                frame.play = false;
            }
        }
    }
}

Character::Character(Scene *scene) : Renderer(scene) {
    rotate.now = quat();
    rotate.moving = false;
    transform.now = vec3(0.0f);
    transform.reset = false;
    transform.moving = false;
    frame.play = false;
    frame.current = 0.0f;
}

template <typename T, typename U>
void updateTransform(T &t, U inter) {
    if (t.moving) {
        if (t.current < t.end) {
            t.now = inter(t.begin, t.target, t.current / t.end);
            ++t.current;
        } else {
            t.now = t.target;
            t.moving = false;
        }
    }
}

void Character::updateMotion() {
    updateTransform(transform, (vec3 (*)(const vec3 &, const vec3 &, float))mix);
    updateTransform(rotate, (quat (*)(const quat &, const quat &, float))slerp);
}

void Character::resume() { frame.play = true; }
void Character::pause() { frame.play = false; }
void Character::play(float start, float speed) {
    frame.begin = frame.current = start;
    frame.speed = speed;
    frame.play = true;
    frame.range = false;
}
float Character::playRange(float start, float end, float speed) {
    frame.begin = frame.current = start;
    frame.end = end;
    frame.speed = speed;
    frame.play = true;
    frame.range = true;
    frame.loop = false;
    return (end - start) / speed * Application::elapse;
}
float Character::playTo(float end, float speed) {
    return playRange(frame.current, end, speed);
}
void Character::loop(float start, float end, float speed) {
    frame.begin = frame.current = start;
    frame.end = end;
    frame.speed = speed;
    frame.play = true;
    frame.range = true;
    frame.loop = true;
}
void Character::loopTo(float end, float speed) {
    loop(frame.current, end, speed);
}
void Character::fix(float start) {
    frame.current = start;
    frame.play = false;
}

void Character::teleport(vec3 pos) {
    transform.now = pos;
    transform.reset = true;
}

void Character::move(vec3 pos) {
    transform.now = pos;
}

float Character::moveTo(vec3 pos, float speed) {
    vec3 cur = transform.now;
    transform.end = distance(pos, cur) / speed / Application::elapse;
    transform.current = 0.0f;
    transform.begin = cur;
    transform.target = pos;
    transform.moving = true;
    return transform.end * Application::elapse;
}

void Character::lookAt(quat dir) {
    rotate.now = dir;
}

float Character::rotateTo(quat dir, float speed) {
    float dis = acos(dot(dir, rotate.now));
    rotate.end = dis / speed / Application::elapse;
    rotate.current = 0.0f;
    rotate.begin = rotate.now;
    rotate.target = dir;
    rotate.moving = true;
    return rotate.end * Application::elapse;
}

float Character::rotateLocal(quat r, float speed) {
    return rotateTo(rotate.now * r, speed);
}

float Character::rotateGlobal(quat r, float speed) {
    return rotateTo(r * rotate.now, speed);
}
