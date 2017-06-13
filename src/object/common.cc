#include "ext.hpp"
#include "renderer.hpp"

using namespace glm;

Renderer::Renderer(Scene *scene) : scene(scene) {}
Renderer::~Renderer() {}
void Renderer::setup(void) {}
void Renderer::update(void) {}
void Renderer::reset(void) {}

Transformer::Transformer(Scene *scene) : Renderer(scene) {}
void Transformer::render() {}

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
    transform.mat = mat4(1.0f);
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
            setMatTranslate(transform.mat, tar);
            ++transform.current;
        } else {
            setMatTranslate(transform.mat, transform.target);
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
void Character::playRange(float start, float end, float speed) {
    frame.begin = frame.current = start;
    frame.end = end;
    frame.speed = speed;
    frame.play = true;
    frame.range = true;
    frame.loop = false;
}
void Character::loop(float start, float end, float speed) {
    frame.begin = frame.current = start;
    frame.end = end;
    frame.speed = speed;
    frame.play = true;
    frame.range = true;
    frame.loop = true;
}
void Character::fix(float start) {
    frame.current = start;
    frame.play = false;
}

void Character::teleport(vec3 pos) {
    transform.mat = translate(pos);
    transform.reset = true;
}

void Character::move(vec3 pos) {
    transform.mat = translate(pos);
}

float Character::moveTo(vec3 pos, float speed) {
    vec3 cur = getMatTranslate(transform.mat);
    transform.end = distance(pos, cur) / speed / Application::elapse;
    transform.current = 0.0f;
    transform.begin = cur;
    transform.target = pos;
    transform.moving = true;
    return transform.end * Application::elapse;
}
