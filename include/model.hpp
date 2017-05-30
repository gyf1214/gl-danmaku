#ifndef __MODEL
#define __MODEL

#include "common.hpp"
#include "mmd/pmx.hpp"

class Model : public mmd::pmx::Model {
    bool loaded;
    std::vector<GLuint> textureSlot;
public:
    Model();
    GLuint texture(int index);
    void load(const char *path);
    void loadTextures(void);
    
    static Model *reimu(void);
};

#endif
