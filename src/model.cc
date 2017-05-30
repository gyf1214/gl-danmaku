#include "ext.hpp"
#include "model.hpp"
#include "texture.hpp"

using namespace mmd;

Model::Model() : mmd::pmx::Model(), loaded(false) {}

void Model::load(const char *path) {
    CHECK(!loaded) << "cannot load a model twice!";
    loaded = true;
    Fs *fs = Fs::open(path);
    mmd::pmx::Model::load(fs);
    delete fs;

    textureSlot.resize(textures.size(), 0);
}

void Model::loadTextures() {
    Texture::white();
    for (int i = 0; i < textures.size(); ++i) {
        texture(i);
    }
}

GLuint Model::texture(int index) {
    CHECK(index >= -1 && index < textures.size()) << "texture index out of bound!";
    if (!textureSlot[index]) {
        textureSlot[index] = Texture::loadTexture(textures[index].c_str());
    }
    return textureSlot[index];
}

#define defineModel(name, path) static Model name##_model;\
Model *Model::name() {\
    if (!name##_model.loaded) {\
        name##_model.load("assets/" path);\
    }\
    return &name##_model;\
}

defineModel(reimu, "reimu.pmx");
