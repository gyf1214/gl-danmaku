#include "ext.hpp"
#include "motion.hpp"
#include <glm/gtc/quaternion.hpp>

using namespace mmd;
using namespace glm;

Motion::Motion() : vmd::Motion(), loaded(false) {}

void Motion::load(const char *path) {
    CHECK(!loaded) << "cannot load a motion twice!";
    loaded = true;
    Fs *fs = Fs::open(path);
    mmd::vmd::Motion::load(fs);
    delete fs;
}

void Motion::loadModel(Model *model) {
    int n = model->bones.size();
    boneRemap.resize(n, NULL);

    for (int i = 0; i < n; ++i) {
        boneRemap[i] = &bones[model->bones[i].name];
        LOG << "map bone " << i << " keyframes: " << boneRemap[i]->size();
    }
}

mat4 Motion::getKey(int frame, int bone) {
    const auto &keys = *boneRemap[bone];
    int l = 0, r = keys.size();
    if (!r) return mat4(1.0f);

    while (r - l > 1) {
        int m = (l + r) / 2;
        if (keys[m]->frame <= frame) {
            l = m;
        } else {
            r = m;
        }
    }

    vec4 rr = keys[l]->rotation;
    quat q = quat(rr.w, rr.x, rr.y, rr.z);
    vec3 pos = keys[l]->position;
    if (r < keys.size()) {
        float scalar = (float)(frame - keys[l]->frame) /
                       (float)(keys[r]->frame - keys[l]->frame);
        rr = keys[r]->rotation;
        quat qq = quat(rr.w, rr.x, rr.y, rr.z);
        q = slerp(q, qq, scalar);
        pos = mix(pos, keys[r]->position, scalar);
    }

    return translate(pos) * mat4_cast(q);
    // return translate(pos);
    // return mat4_cast(quat(q.w, q.x, q.y, q.z));
}

#define defineMotion(name, path) Motion *Motion::name() {\
    static Motion motion;\
    if (!motion.loaded) {\
        motion.load("assets/" path);\
    }\
    return &motion;\
}

defineMotion(test1, "test1.vmd");
