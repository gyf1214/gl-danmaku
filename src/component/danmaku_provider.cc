#include "../ext.hpp"
#include "component/provider.hpp"
#include "vertex/danmaku.hpp"
#include "../builder/common.hpp"
#include <array>

using namespace Builder;

class DanmakuProvider : public virtual Provider {
protected:
    std::vector<Vertex> pool;
public:
    virtual void setupDanmaku(void) = 0;
    void setup() {
        LOG << "setup danmaku vertices";
        srand(time(NULL));
        pool.resize(0);
        setupDanmaku();
        LOG << "done setup danmaku vertices";
    }
    void reset() {
        LOG << "reset danmaku provider";
        pool.clear();
    }
    size_t size() const { return sizeof(Vertex) * pool.size(); }
    const void *data() const { return pool.data(); }
};

class Danmaku1 : public DanmakuProvider {
public:
    void setupDanmaku() {
        Base *src = source(pool)->set();

        // KEEP: utsuho-1-non

        // Chain(generator(1))
        //     << point(0.0f, -20.0f, 60.0f)
        //     << sphere(10, 10)
        //     << generator(20)
        //     << emitter(0.0f, 0.1f)
        //     << type(0, 16, 8, 1.0f)
        //     << linearSpeed(10.0f, 0.0f)
        //     << dieAfter(10.0f)
        //     << src << Emit();

        // KEEP: self-1

        // Chain(generator(3))
        //     << line(vec3(-1.0f, 20.0f, 60.0f), vec3(1.0f, 0.0f, 0.0f))
        //     << generator(20)
        //     << emitter(0.0f, 0.1f)
        //     << type(8, 16, 4, 0.5f)
        //     << direction(vec3(0.0f, -20.0f, 0.0f))
        //     << dieAfter(10.0f)
        //     << src << Emit();

        // KEEP: utsuho-1-card

        // Chain(generator(1))
        //     << point(0.0f, -20.0f, 60.0f)
        //     << emitter(0.0f, 0.0f)
        //     << sphere(0, 1)
        //     << type(0, 0, 16, 20.0f)
        //     << linearSpeed(0.7f, 0.0f)
        //     << dieAfter(100.0f)
        //     << src << Emit();
        //
        // Chain(generator(30))
        //     << point(0.0f, -20.0f, 60.0f)
        //     << generator(20)
        //     << emitter(1.0f, 0.5f)
        //     << randomDirection()
        //     << type(0, 24, 8, 2.0f)
        //     << linearSpeed(4.0f, 0.0f)
        //     << dieAfter(100.0f)
        //     << src << Emit();


        // Chain(generator(10))
        //     << line(vec3(-2.0f, 5.0f, 20.0f),
        //             vec3(4.0f, 0.0f, 0.0f) / 5.0f)
        //     << emitter(0.0f, 0.1f)
        //     << type(0, 24, 8, 1.0f)
        //     << direction(0.0f, 0.0f, -1.0f)
        //     << generator(20)
        //     << linearSpeed(0.5f, 0.5f)
        //     << dieAfter(60.0f)
        //     << src << Emit();
        //

        // Chain(generator(1))
        //     << point(0.0f, 0.0f, 20.0f)
        //     << direction(5.0f, 0.0f, 0.0f)
        //     << circleMotion(0.0f, 10.0f, 20.0f)
        //     << generator(20)
        //     << emitter(2.0f, 0.05f)
        //     << type(0, 16, 8, 0.3f)
        //     << src << Emit();

        // Multi circles;
        // const int N = 3;
        // const int M = 3;
        //
        // for (int k = -M; k <= M; ++k) {
        //     float pitch = (float)k / (float)(M + 1) * M_PI / 2.0f;
        //     int T = 4 * (1 + floor(N * cos(pitch)));
        //     for (int i = 0; i < T; ++i) {
        //         float angle = (float)i / (float)T * M_PI * 2.0f;
        //         vec3 x = vec3(cos(angle) * cos(pitch),
        //                     sin(angle) * cos(pitch), 0.0f) * 10.0f;
        //         vec3 y = vec3(-sin(angle) * cos(pitch),
        //                     cos(angle) * cos(pitch), 0.0f) * 10.0f;
        //
        //         Multi m = Multi()
        //             << circle(x, y,  0.03f)
        //             << circle(x, y, -0.03f);
        //         Chain c = Chain(m)
        //             << addVelocity(vec3(0.0f, 0.0f, sin(pitch)) * 10.0f);
        //         circles << c;
        //     }
        // }

        //
        // Chain(generator(1))
        //     << point(0.0f, 0.0f, 20.0f)
        //     << circle << generator(30)
        //     << emitter(3.0f, 0.05f)
        //     << dieAfter(2.0f)
        //     << type(0, 16, 8, 0.6f)
        //     << src << Emit();

        // Chain(generator(50))
        //     << point(0.0f, 0.0f, 20.0f)
        //     << emitter(0.0f, 0.02f)
        //     << circles << dieAfter(2.0f)
        //     << type(0, 16, 8, 0.6f)
        //     << src << Emit();

        // Multi bias = Multi()
        //     << crossBias(0.0f, 0.0f,  1.0f);
        //     << crossBias(0.0f, 0.0f, -10.0f);
        //
        // Chain(generator(1))
        //     << point(0.0f, 0.0f, 20.0f)
        //     << direction(5.0f, 0.0f, 0.0f)
        //     << sphere(5, 5)
        //     << bias
        //     << generator(30)
        //     << emitter(2.0f, 0.05f)
        //     << type(0, 16, 8, 0.3f)
        //     << src << Emit();

        src->reset();
    }
};

Provider *Provider::danmaku1() {
    return Box::create<Danmaku1>();
}
