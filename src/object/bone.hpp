#ifndef __BONE
#define __BONE

#include "common.hpp"
#include "model.hpp"

struct Bone {
    glm::mat4 transform;
    int son, brother, parent;

    Bone() : transform(1.0f), son(-1), brother(-1) {}
};

class Armature {
    Model *model;
    std::vector<Bone> bones;
    std::vector<glm::mat4> output;
    int root;

    void dfs(int v);
public:
    Armature(Model *model) : model(model) {}
    void setup();

    std::vector<glm::mat4> &transform() { return output; }
    const std::vector<glm::mat4> &transform() const { return output; }
    glm::mat4 &operator [](int index) { return bones[index].transform; }
    const glm::mat4 &operator [](int index) const {
        return bones[index].transform;
    }
    int size() { return bones.size(); }

    void buildTree(void);
    void applyTransform(void) { dfs(root); }
};

#endif
