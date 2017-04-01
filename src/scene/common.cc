#include "scene.hpp"

Scene::~Scene() {}
void Scene::setup(void)  { for (auto o : objects) if (o) o -> setup(); }
void Scene::update(void) { for (auto o : objects) if (o) o -> update(); }
void Scene::render(void) { for (auto o : objects) if (o) o -> render(); }
void Scene::reset(void)  { for (auto o : objects) ObjectBox::release(o); }
void Scene::onKey(int key, int scancode, int action, int mods) {}
