#include "../ext.hpp"
#include "component/model.hpp"
#include "component/texture.hpp"

using namespace glm;
using namespace mmd;

class ModelImp : private MMDModel, public virtual Model {
    std::vector<GLuint> textureSlot;
    GLuint morphBuf, morphTex, white;
public:
    ModelImp(const char *path) {
        LOG << "load model from path: " << path;
        Fs *fs = Fs::open(path);
        MMDModel::load(fs);
        delete fs;
        LOG << "model name: " << header.name;

        int n = textures.size();
        LOG << "load " << n << " textures";
        textureSlot.resize(n, 0);
        white = Texture::white();
        for (int i = 0; i < n; ++i) {
            textureSlot[i] = Texture::loadTexture(textures[i].c_str());
            LOG << "texture " << i << ": " << textureSlot[i];
        }

        LOG << "load morph texture";
        glGenBuffers(1, &morphBuf);
        glBindBuffer(GL_TEXTURE_BUFFER, morphBuf);
        LOG << "texture buffer: " << morphBuf;
        int m = morphs.size();
        n = mesh.vertex.size();
        vec4 *data = new vec4[n * m];
        LOG << "size: " << n << "x" << m;
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
        LOG << "morph texture: " << morphTex;
        glBindTexture(GL_TEXTURE_BUFFER, morphTex);
        glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, morphBuf);
    }

    void setup() {}
    void reset() {}

    ~ModelImp() {
        LOG << "reset model: " << header.name;
        int n = textureSlot.size();
        LOG << "delete " << n << " textures";
        for (int i = 0; i < n; ++i) {
            LOG << "delete texture " << i << ": " << textureSlot[i];
            glDeleteTextures(1, &textureSlot[i]);
        }
        textureSlot.clear();
        LOG << "delete morph texture: " << morphTex;
        glDeleteTextures(1, &morphTex);
        LOG << "delete morph buffer: " << morphBuf;
        glDeleteBuffers(1, &morphBuf);
    }

    GLuint texture(int index) const {
        CHECK(index >= -1 && index < textures.size()) << "invalid texture slot!";
        return index >= 0 ? textureSlot[index] : white;
    }

    GLuint morphTexture() const { return morphTex; }
    const MMDModel *mmdModel() const { return this; }
};

#define defineModel(name, path) Model *Model::name() {\
    static Model *model = NULL;\
    if (!model) model = Box::global<ModelImp>("assets/" path);\
    return model;\
}

#define defineMotion(name, path) MMDMotion *Model::name() {\
    static MMDMotion motion;\
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
defineModel(utsuho, "utsuho.pmx");
defineModel(suwako, "suwako.pmx");
defineMotion(test1, "test1.vmd");
defineMotion(test2, "test2.vmd");
