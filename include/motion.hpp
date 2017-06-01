#ifndef __MOTION
#define __MOTION

#include "model.hpp"
#include "mmd/vmd.hpp"

class Motion : public mmd::vmd::Motion {
    bool loaded;
    std::vector<std::vector<mmd::vmd::Keyframe*>*> boneRemap;
    Motion();
public:
    void load(const char *path);
    void loadModel(Model *model);
    glm::mat4 getKey(int frame, int bone);

    static Motion *test1(void);
};

#endif
