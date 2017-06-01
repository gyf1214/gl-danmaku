#include "ext.hpp"
#include "model.hpp"
#include "texture.hpp"

using namespace glm;
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

mat4 Model::boneTransform(int index, const mat4 &trans) {
    vec3 pos = bones[index].position;
    return translate(pos) * trans * translate(-pos);
}

#define defineModel(name, path) Model *Model::name() {\
    static Model model;\
    if (!model.loaded) {\
        model.load("assets/" path);\
    }\
    return &model;\
}

defineModel(reimu, "reimu.pmx");
