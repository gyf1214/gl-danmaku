#ifndef __MODEL
#define __MODEL

#include "common.hpp"
#include "mmd/pmx.hpp"

class Model : public mmd::pmx::Model {
    bool loaded;
    std::vector<GLuint> textureSlot;
    GLuint morphBuffer, morphTex;
    void load(const char *path);
    Model();
public:
    GLuint texture(int index);
    GLuint morphTexture(void);
    void loadTextures(void);
    void loadMorphTexture(void);

    static Model *reimu(void);
};

#endif
