#ifndef __MODEL
#define __MODEL

#include "common.hpp"

class Model : public mmd::pmx::Model {
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
    static mmd::vmd::Motion *test1(void);
    static mmd::vmd::Motion *test2(void);
};

#endif
