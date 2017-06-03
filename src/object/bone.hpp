#ifndef __BONE
#define __BONE

#include "common.hpp"
#include "model.hpp"
#include "mmd-physics/ik.hpp"

struct Bone {
    glm::mat4 transform;
    int parent, son, brother;
    int depth;
    bool updated;

    Bone() : transform(1.0f), parent(-1),
             son(-1), brother(-1), updated(false) {}
};

class Armature {
    Model *model;
    std::vector<Bone> bones;
    std::vector<glm::mat4> output;
    std::vector<int> list;
    mmd::physics::IK *ik;

    void buildTree(void);
    void dfs(int v);
    void dfsApply(int v, bool brother);
    void applyIK(int v);

    struct BoneComp;
public:
    Armature(Model *model) : model(model), ik(NULL) {}
    ~Armature();
    void setup();
    void applyTransform(void);

    std::vector<glm::mat4> &transform() { return output; }
    const std::vector<glm::mat4> &transform() const { return output; }
    glm::mat4 &operator [](int index) { return bones[index].transform; }
    const glm::mat4 &operator [](int index) const {
        return bones[index].transform;
    }
    int size() const { return bones.size(); }
    glm::vec3 getBonePosition(int v) const {
        return output[v] * vec4(model->bones[v].position, 1.0f);
    }
};

#endif
