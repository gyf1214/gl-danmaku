#include "../ext.hpp"
#include "component/character.hpp"
#include "component/model.hpp"
#include "basic_motion.hpp"
#include "mmd-physics/motion.hpp"

using namespace std;
using namespace glm;

class KeyImp : private MotionData<float>, public virtual Keyframe {
    float doPlay(float start, float stop, float speed, bool loop) {
        playing = true;
        looping = loop;
        origin = start;
        target = stop;
        current = 0.0f;
        end = (stop - start) / speed;
        return end * Application::elapse;
    }
public:
    KeyImp() {
        playing = looping = false;
        now = 0.0f;
    }

    void setup() {}
    void reset() {}
    void update() {
        MotionData::update((float (*)(float, float, float))mix);
    }

    float frame() const { return now; }

    float play(float start, float stop, float speed) {
        return doPlay(start, stop, speed, false);
    }
    float playTo(float stop, float speed) {
        return doPlay(now, stop, speed, false);
    }
    void loop(float start, float stop, float speed) {
        doPlay(start, stop, speed, true);
    }
    void loopTo(float stop, float speed) { doPlay(now, stop, speed, true); }
    void fix(float start) {
        now = start;
        looping = playing = false;
    }
    void pause() { looping = playing = false; }
};

class CharacterImp : public BasicMotion, public KeyImp, public virtual Character {
    static const mat4 preTransform;
    static const mat4 invTransform;

    Model *cModel;
    MMDMotion *vMotion;
    mmd::physics::Motion *motion;

    vector<float> morphs;
    vector<mat4> bones;

    bool resetPhysics;

    mat4 localTransform() {
        return invTransform * matrix();
    }
public:
    CharacterImp(Model *cModel, MMDMotion *vMotion, bool debug = false)
        : cModel(cModel), vMotion(vMotion) {
        motion = mmd::physics::Motion::create(debug);
    }

    ~CharacterImp() {
        delete motion;
    }

    void setup() {
        LOG << "setup character";

        LOG << "setup basic motion & key";
        BasicMotion::setup();
        KeyImp::setup();

        const MMDModel *mModel = cModel->mmdModel();
        LOG << "MMD model: " << mModel->header.name;
        LOG << "MMD motion: " << vMotion->header.name;

        LOG << "setup physics";
        motion->loadModel(mModel);
        motion->loadMotion(vMotion);
        motion->updateGlobal(localTransform());
        motion->updateKey(frame());
        motion->loadBody();
        resetPhysics = false;

        bones.resize(mModel->bones.size());
        morphs.resize(mModel->morphs.size());
    }

    void reset() {
        LOG << "reset character";

        LOG << "reset basic motion & key";
        BasicMotion::reset();
        KeyImp::reset();

        LOG << "reset physics";
        motion->reset();
    }

    void update() {
        BasicMotion::update();
        KeyImp::update();

        motion->updateKey(frame());
        motion->updateGlobal(localTransform());
        if (resetPhysics) {
            motion->resetPhysics();
            resetPhysics = false;
        }
        motion->updatePhysics(Application::elapse);

        for (int i = 0; i < bones.size(); ++i) {
            bones[i] = motion->skin(i);
        }
        for (int i = 0; i < morphs.size(); ++i) {
            morphs[i] = motion->face(i);
        }
    }

    mat4 matrix() const {
        return BasicMotion::matrix() * preTransform;
    }

    void teleport(vec3 pos) {
        BasicMotion::teleport(pos);
        resetPhysics = true;
    }

    const vector<mat4> &skins() const { return bones; }
    const vector<float> &faces() const { return morphs; }
    const Model *model() const { return cModel; }
private:
    class BindPoint : public virtual Translate {
        const CharacterImp *ch;
        int bone;
        vec3 origin;
    public:
        BindPoint(const CharacterImp *ch, int bone, vec3 origin)
            : ch(ch), bone(bone), origin(origin) {}

        void setup() {
            LOG << "setup bind point from character: "
                << ch->cModel->mmdModel()->header.name;
        }
        void reset() {
            LOG << "reset bind point";
        }
        void update() {}

        vec3 position() const {
            if (bone >= 0) {
                return ch->matrix() * ch->bones[bone] * vec4(origin, 1.0f);
            } else {
                return ch->matrix() * vec4(origin, 1.0f);
            }
        }
    };
public:
    Translate *bindPoint(int bone, vec3 origin) const {
        return new BindPoint(this, bone, origin);
    }
};

const mat4 CharacterImp::preTransform(
    0.1f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.1f, 0.0f,
    0.0f, 0.1f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
);

const mat4 CharacterImp::invTransform = inverse(preTransform);

Character *Character::basic(Model *model, MMDMotion *motion) {
    return Box::create<CharacterImp>(model, motion);
}
