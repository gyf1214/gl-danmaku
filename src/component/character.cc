#include "../ext.hpp"
#include "component/character.hpp"
#include "component/model.hpp"
#include "basic_motion.hpp"
#include "mmd-physics/motion.hpp"

using namespace std;
using namespace glm;

const static float mmdElapse = 1.0f / 30.0f;

class KeyImp : private MotionData<float>, public virtual Keyframe {
    float doPlay(float start, float stop, float speed, bool loop) {
        playing = true;
        looping = loop;
        origin = start;
        target = stop;
        current = 0.0f;
        end = (stop - start) / speed * mmdElapse;
        return end;
    }
public:
    KeyImp() {
        playing = looping = false;
        now = 0.0f;
    }

    void setup() {}
    void reset() {}
    void update() {
        MotionData::update((InterFloat)mix);
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

class SplineMotion : public BasicMotion {
    vector<vec3> points;
    vector<float> lines;
    float current, sum;
    int now;
    bool autoDir;
public:
    void setup() {
        points.clear();
        lines.clear();
        current = 0.0f;
        autoDir = false;

        BasicMotion::setup();
    }

    void update() {
        BasicMotion::update();

        if (!lines.empty()) {
            current += Application::elapse();
            int n = lines.size();
            for (; now < n && current > lines[now]; ++now) current -= lines[now];
            bool flag = false;
            if (now >= n) {
                now = n - 1;
                current = lines[now];
                flag = true;
            }
            pos.now = catmullRom(points[now], points[now + 1], points[now + 2],
                                 points[now + 3], current / lines[now]);
            if (autoDir) {
                vec3 dir = catmullRomNorm(points[now], points[now + 1],
                           points[now + 2], points[now + 3], current / lines[now]);
                rot.now = quat(vec3(0.0f, 0.0f, atan2(dir.x, -dir.y)));
            }
            if (flag) { lines.clear(); points.clear(); }
        }
    }

    float waypoint(vec3 p, float time) {
        if (lines.empty()) {
            lines.push_back(time);
            sum = time;
            current = 0.0f;
            now = 0;
            points.push_back(pos.now);
            points.push_back(pos.now);
            points.push_back(p);
            points.push_back(p);
        } else {
            sum += time;
            lines.push_back(time);
            int n = points.size();
            points[n - 1] = p;
            points.push_back(p);
        }
        return sum;
    }

    void autoRot(bool e) { autoDir = e; }
};

class CharacterImp : public SplineMotion, public KeyImp, public virtual Character {
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
        SplineMotion::setup();
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
        SplineMotion::reset();
        KeyImp::reset();

        LOG << "reset physics";
        motion->reset();
    }

    void update() {
        SplineMotion::update();
        KeyImp::update();

        motion->updateKey(frame());
        motion->updateGlobal(localTransform());
        if (resetPhysics) {
            motion->resetPhysics();
            resetPhysics = false;
        }
        motion->updatePhysics(Application::elapse());

        for (int i = 0; i < bones.size(); ++i) {
            bones[i] = motion->skin(i);
        }
        for (int i = 0; i < morphs.size(); ++i) {
            morphs[i] = motion->face(i);
        }
    }

    mat4 matrix() const {
        return SplineMotion::matrix() * preTransform;
    }

    void teleport(vec3 pos) {
        SplineMotion::teleport(pos);
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

Motion *Motion::spline() {
    return Box::create<SplineMotion>();
}
