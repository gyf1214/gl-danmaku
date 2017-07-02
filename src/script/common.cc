#include "common.hpp"

void BasicScript::doFiber(void *self) {
    reinterpret_cast<BasicScript *>(self)->run();
}

void BasicScript::pushBase(BaseObject *c) { tracks.push_back(c); }
void BasicScript::pushBase(Component *c) { coms.push_back(c); tracks.push_back(c); }
void BasicScript::pushBase(Object *c) { root->push(c); }

void BasicScript::await() {
    waiting = true;
    nextEvent = frame;
}

void BasicScript::await(float x) {
    waiting = true;
    nextEvent = frame + x / Application::elapse;
    Fiber::yield();
}

void BasicScript::awaitUntil(float x) {
    waiting = true;
    nextEvent = x / Application::elapse;
    Fiber::yield();
}

void BasicScript::setup() {
    nextEvent = 0.0f;
    waiting = false;
    fiber = Fiber::create(doFiber, this);
    fiber->resume();

    for (const auto &c : tracks) { c->setup(); }
    // for (const auto &c : coms) { c->setup(); }
}

void BasicScript::reset() {
    LOG << "reset script";
    delete fiber;
    fiber = NULL;

    // for (const auto &c : coms) { Box::release(c); }
    for (const auto &c : tracks) { Box::release(c); }
    tracks.clear();
    coms.clear();
}

void BasicScript::update() {
    if (waiting && frame >= nextEvent) {
        waiting = false;
        LOG << "event triggered at: " << frame;
        fiber->resume();
    }

    for (const auto &c : coms) { c->update(); }
}
