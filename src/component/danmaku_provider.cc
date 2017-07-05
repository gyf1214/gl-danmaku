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
        LOG << "danmaku size: " << pool.size();
    }
    void reset() {
        LOG << "reset danmaku provider";
        pool.clear();
    }
    size_t size() const { return sizeof(Vertex) * pool.size(); }
    const void *data() const { return pool.data(); }
};

static Chain utsuho_1_non() {
    return Chain(sphereSpread(10, 10))
        << generator(20)
        << emitter(0.0f, 0.1f)
        << type(0, 16, 8, 1.0f)
        << linearSpeed(10.0f, 0.0f)
        << dieAfter(20.0f);
}

static Chain self_1() {
    return Chain(generator(3))
        << line(vec3(-0.5f, 0.0f, 0.0f), vec3(0.5f, 0.0f, 0.0f))
        << generator(20)
        << emitter(0.0f, 0.1f)
        << type(8, 16, 4, 0.5f)
        << direction(vec3(0.0f, -20.0f, 0.0f))
        << dieAfter(2.0f);
}

static Chain utsuho_1_card() {
    Chain c1 = Chain(sphereSpread(0, 1))
        << type(0, 0, 16, 10.0f)
        << linearSpeed(0.8f, 0.0f);

    Chain c2 = Chain(generator(30))
        << generator(15)
        << emitter(1.0f, 0.5f)
        << randomDirection()
        << type(0, 24, 8, 2.0f)
        << linearSpeed(4.0f, 0.0f);

    return Chain(Multi() << c1 << c2)
        << dieAfter(12.0f);
}

static Chain utsuho_2_non_single(vec3 dir) {
    vec3 k = glm::sphericalRand(1.0f);
    k = normalize(k - dot(k, dir) * dir);
    vec3 t = cross(k, dir);

    return Chain(generator(3))
        << circleSpread(k, t, M_PI / 1.5f);
}

static Chain utsuho_2_non_spline(const vec3 &p0, const vec3 &p1,
                                 const vec3 &p2, const vec3 &p3,
                                 int N, float elapse) {
    Multi gens;
    for (int i = 0; i < N; ++i) {
        vec3 pos = catmullRom(p0, p1, p2, p3, (float)i / (float)N);
        vec3 dir = catmullRomNorm(p0, p1, p2, p3, (float)i / (float)N);

        gens << (utsuho_2_non_single(dir)
                 << addPosition(pos)
                 << addTime((float)i / (float)N * elapse));
    }
    return Chain(gens);
}

static Chain utsuho_2_non_common() {
    return Chain(generator(15))
        << type(8, 24, 8, 2.0f)
        << linearSpeed(0.5f, 0.8f)
        << dieAfter(100.0f);
}

static Chain utsuho_2_card() {
    Chain c1 = Chain(generator(100))
        << randomBox(vec3(-25.0f, -25.0f, 40.0f), vec3(25.0f, 25.0f, 50.0f))
        << emitter(0.0f, 0.2f)
        << direction(0.0f, 0.0f, -10.0f)
        << type(0, 0, 16, 10.0f);

    Chain c2 = Chain(generator(50))
        << randomBox(vec3(-10.0f, -10.0f, 40.0f), vec3(10.0f, 10.0f, 50.0f))
        << emitter(0.0f, 0.5f)
        << generator(20)
        << randomDirection()
        << linearSpeed(5.0f, 0.0f)
        << addVelocity(0.0f, 0.0f, -10.0f)
        << type(0, 24, 8, 2.0f);

    return Chain(Multi() << c1 << c2)
        << dieAfter(30.0f);
}

static Chain utsuho_2_extra() {
    return Chain(generator(100))
        << randomDirection()
        << linearSpeed(8.0f, 0.0f)
        << generator(10)
        << emitter(0.0f, 0.5f)
        << type(16, 24, 8, 5.0f)
        << dieAfter(30.0f);
}

static Chain utsuho_3_card() {
    Multi tens;
    tens << (Chain(circleSpread(vec3(1.0f, 0.0f, 0.0f),
                                vec3(0.0f, 1.0f, 0.0f), M_PI / 2.5f))
             << linearSpeed(4.0f, 0.0f)
             << dieAfter(2.0f))

         << (Chain(circleSpread(vec3(0.0f, 1.0f, 0.0f),
                                vec3(1.0f, 0.0f, 0.0f), M_PI / 2.5f))
             << linearSpeed(10.0f, 0.0f)
             << dieAfter(2.0f))

         << (Chain(circle(vec3(8.0f, 0.0f, 0.0f),
                          vec3(0.0f, 8.0f, 0.0f), M_PI / 2.5f))
             << emitter(2.0f, 0.0f)
             << circleSpread(vec3(0.0f, -2.0f, 0.0f),
                             vec3(2.0f,  0.0f, 0.0f), M_PI / 2.5f)
             << circleMotion(vec3(0.0f, 0.0f, 0.0f))
             << dieAfter(50.0f))

         << (Chain(circle(vec3(0.0f, 20.0f, 0.0f),
                          vec3(20.0f, 0.0f, 0.0f), M_PI / 2.5f))
             << emitter(2.0f, 0.0f)
             << circleSpread(vec3(-2.0f, 0.0f, 0.0f),
                             vec3( 0.0f, 2.0f, 0.0f), M_PI / 2.5f)
             << circleMotion(vec3(0.0f, 0.0f, 0.0f))
             << dieAfter(50.0f));

    Multi all = Multi()
        << (Chain(generator(20))
            << emitter(1.0f, 1.0f)
            << generator(100)
            << randomDirection()
            << linearSpeed(5.0f, 0.0f)
            << type(8, 24, 8, 2.0f)
            << dieAfter(50.0f))
        << (Chain(generator(5)) << tens << type(0, 0, 16, 15.0f));

    return Chain(all);
}

class Danmaku1 : public DanmakuProvider {
public:
    void setupDanmaku() {
        Emit dst = Emit(source(pool));

        float now = 0.0f;

        // KEEP: utsuho-1-non

        now += 5.0f;

        utsuho_1_non() << addPosition(0.0f, -20.0f, 60.0f)
                       << addTime(now) << dst;

        // KEEP: self-1

        now += 4.0f;

        self_1() << addPosition(1.0f, 20.0f, 60.0f)
                 << addTime(now) << dst;

        // KEEP: utsuho-1-card

        now += 6.0f;

        utsuho_1_card() << addPosition(0.0f, -10.0f, 60.0f)
                        << addTime(now) << dst;

        // KEEP: utsuho-2-non

        now += 12.0f;

        vec3 p0(18.0f, -15.0f, 57.5f);
        vec3 p1(20.0f, -5.0f, 57.5f);
        vec3 p2(22.0f,  5.0f, 58.0f);
        vec3 p3(20.0f,  15.0f, 58.0f);

        Multi splines;

        splines << utsuho_2_non_spline(p0, p0, p1, p2, 2, 1.0f)
                << (Chain(utsuho_2_non_spline(p0, p1, p2, p3, 2, 1.0f))
                    << addTime(1.0f))
                << (Chain(utsuho_2_non_spline(p1, p2, p3, p3, 2, 1.0f))
                    << addTime(2.0f));

        Chain(splines)
            << utsuho_2_non_common() << addTime(now) << dst;


        // vec3 p0 = vec3(-60.0f,  10.0f, 60.0f);
        // vec3 p1 = vec3(-30.0f, -20.0f, 60.0f);
        // vec3 p2 = vec3( 30.0f, -20.0f, 60.0f);
        // vec3 p3 = vec3( 60.0f,  10.0f, 60.0f);
        // utsuho_2_non_spline(p0, p1, p2, p3, 10, 2.0f)
        //     << utsuho_2_non_common() << addTime(2.0f) << dst;

        // KEEP: utsuho-2-card

        // utsuho_2_card() << addPosition(0.0f, 0.0f, 60.0f)
        //                 << addTime(2.0f) << dst;

        // KEEP: utsuho-2-extra

        // utsuho_2_extra() << addPosition(0.0f, 0.0f, 60.0f)
        //                  << addTime(2.0f) << dst;

        // KEEP: utsuho-3-card

        // utsuho_3_card() << addPosition(0.0f, 0.0f, 60.0f)
        //                 << addTime(2.0f) << dst;

        // END: KEEP

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
    }
};

Provider *Provider::danmaku1() {
    return Box::create<Danmaku1>();
}
