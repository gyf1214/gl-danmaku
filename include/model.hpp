#ifndef __MODEL
#define __MODEL

#include "common.hpp"
#include "mmd/pmx.hpp"

class Model : public mmd::pmx::Model {
    bool loaded;
    std::vector<GLuint> textureSlot;
    Model();
public:
    GLuint texture(int index);
    void load(const char *path);
    void loadTextures(void);

    glm::mat4 boneTransform(int index, const glm::mat4 &trans);

    static Model *reimu(void);
};

#endif
