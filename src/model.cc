#include "ext.hpp"
#include "model.hpp"
#include "texture.hpp"

using namespace glm;
using namespace mmd;

Model::Model() : mmd::pmx::Model(), morphTex(0) {}

void Model::load(const char *path) {
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

GLuint Model::morphTexture() {
    if (!morphTex) loadMorphTexture();
    return morphTex;
}

void Model::loadMorphTexture() {
    glGenBuffers(1, &morphBuffer);
    glBindBuffer(GL_TEXTURE_BUFFER, morphBuffer);
    int m = morphs.size();
    int n = mesh.vertex.size();
    vec4 *data = new vec4[n * m];
    memset(data, 0, n * m * sizeof(vec4));
    for (int i = 0; i < m; ++i) {
        const auto &offset = morphs[i].offsets;
        int t = offset.size();
        for (int j = 0; j < t; ++j) {
            data[offset[j].index * m + i] = vec4(offset[j].translate, 0.0f);
        }
    }
    glBufferData(GL_TEXTURE_BUFFER, n * m * sizeof(vec4), data, GL_STATIC_DRAW);
    delete[] data;

    glGenTextures(1, &morphTex);
    glBindTexture(GL_TEXTURE_BUFFER, morphTex);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, morphBuffer);
}

#define defineModel(name, path) Model *Model::name() {\
    static Model model;\
    static bool loaded = false;\
    if (!loaded) {\
        model.load("assets/" path);\
        loaded = true;\
    }\
    return &model;\
}

#define defineMotion(name, path) vmd::Motion *Model::name() {\
    static vmd::Motion motion;\
    static bool loaded = false;\
    if (!loaded) {\
        Fs *fs = mmd::Fs::open("assets/" path);\
        motion.load(fs);\
        loaded = true;\
        delete fs;\
    }\
    return &motion;\
}

defineModel(reimu, "reimu.pmx");
defineMotion(test1, "test1.vmd");
defineMotion(test2, "test2.vmd");
