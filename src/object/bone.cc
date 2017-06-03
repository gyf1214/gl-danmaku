#include "bone.hpp"
#include "../ext.hpp"

using namespace glm;
using namespace mmd;
using namespace mmd::physics;

struct Armature::BoneComp {
    Armature *self;
    BoneComp(Armature *self) : self(self) {}
    bool operator ()(int a, int b) {
        int ord1 = self->model->bones[a].order;
        int ord2 = self->model->bones[b].order;
        return ord1 < ord2 || (ord1 == ord2 &&
                self->bones[a].depth < self->bones[b].depth);
    }
};

void Armature::setup() {
    int size = model->bones.size();
    bones.resize(size);
    output.resize(size);
    list.clear();
    buildTree();
    ik = IK::gradient(0.1f, 100);
}

void Armature::applyTransform() {
    for (int i = 0; i < list.size(); ++i) {
        int v = list[i];
        output[v] = model->boneTransform(v, bones[v].transform);
        if (bones[v].parent >= 0) {
            output[v] = output[bones[v].parent] * output[v];
        }
        if (model->bones[v].flag & pmx::Bone::IsIK
         && model->bones[v].flag & pmx::Bone::Enable) {
            applyIK(v);
        }
    }
}

void Armature::applyIK(int v) {
    const auto &mb = model->bones[v];
    CHECK(mb.flag & pmx::Bone::IsIK) << "bone not IK!";
    ik->reset();

    const auto &links = mb.IK.links;
    int n = links.size();
    vec3 pos;
    int u;
    for (int i = n - 1; i >= 0; --i) {
        u = links[i].bone;
        CHECK(!i || bones[links[i - 1].bone].parent == links[i].bone)
            << "invalid ik link: " << v << " -> " << u;
        pos = getBonePosition(u);
        ik->pushBone(pos);
    }
    u = mb.IK.target;
    CHECK(bones[u].parent == links[0].bone) << "invalid ik link: "
                                            << v << " -> " << u;
    pos = getBonePosition(u);
    ik->pushBone(pos);
    pos = getBonePosition(v);
    ik->solve(pos);
    for (int i = 0; i < n; ++i) {
        bones[links[i].bone].transform = ik->getRotation(i);
    }
    dfsApply(links[n - 1].bone, false);
}

void Armature::dfs(int v) {
    auto &b = bones[v];
    if (b.parent >= 0) {
        b.depth = bones[b.parent].depth + 1;
    } else {
        b.depth = 0;
    }

    if (b.son >= 0) dfs(b.son);
    if (b.brother >= 0) dfs(b.brother);
}

void Armature::dfsApply(int v, bool brother) {
    auto &b = bones[v];
    output[v] = model->boneTransform(v, b.transform);
    if (b.parent >= 0) {
        output[v] = output[b.parent] * output[v];
    }

    if (b.son >= 0) dfsApply(b.son, true);
    if (brother && b.brother >= 0) dfsApply(b.brother, true);
}

void Armature::buildTree() {
    int root = -1;
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
        output[i] = mat4(1.0f);
        list.push_back(i);
    }
    if (root >= 0) dfs(root);
    std::sort(list.begin(), list.end(), BoneComp(this));
}

Armature::~Armature() {
    if (ik) delete ik;
}
