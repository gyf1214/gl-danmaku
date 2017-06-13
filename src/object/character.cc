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
    transform.rot = mat4(1.0f);
    transform.pos = vec3(0.0f);
    transform.reset = false;
    transform.moving = false;
    frame.play = false;
    frame.current = 0.0f;
}

void Character::updateMotion() {
    if (transform.moving) {
        if (transform.current < transform.end) {
            vec3 tar = mix(transform.begin, transform.target,
                        transform.current / transform.end);
            transform.pos = tar;
            ++transform.current;
        } else {
            transform.pos = transform.target;
            transform.moving = false;
        }
    }
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
    transform.pos = pos;
    transform.reset = true;
}

void Character::move(vec3 pos) {
    transform.pos = pos;
}

float Character::moveTo(vec3 pos, float speed) {
    vec3 cur = transform.pos;
    transform.end = distance(pos, cur) / speed / Application::elapse;
    transform.current = 0.0f;
    transform.begin = cur;
    transform.target = pos;
    transform.moving = true;
    return transform.end * Application::elapse;
}
