#include "bone.hpp"

void Armature::setup() {
    int size = model->bones.size();
    bones.resize(size);
    output.resize(size);
    root = -1;
}

void Armature::dfs(int v) {
    int p = bones[v].parent;
    output[v] = model->boneTransform(v, bones[v].transform);
    if (p >= 0) output[v] = output[p] * output[v];

    if (bones[v].son >= 0) dfs(bones[v].son);
    if (bones[v].brother >= 0) dfs(bones[v].brother);
}

void Armature::buildTree() {
    for (int i = 0; i < bones.size(); ++i) {
        auto &b = bones[i];
        const auto &mb = model->bones[i];
        b.parent = mb.parent;
        if (b.parent >= 0) {
            auto &p = bones[b.parent];
            b.brother = p.son;
            p.son = i;
        } else {
            b.brother = root;
            root = i;
        }
    }
}
