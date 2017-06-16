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
