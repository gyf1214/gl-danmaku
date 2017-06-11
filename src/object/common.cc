#include "renderer.hpp"

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

Character::Character(Scene *scene) : Renderer(scene) {}

void Character::updateMotion() {
    // TODO
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
