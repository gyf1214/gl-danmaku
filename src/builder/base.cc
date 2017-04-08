#include "builder.hpp"
#include "util.hpp"

namespace Builder {
    class Source : public Base {
        Vertex *now;
        int cnt;
    public:
        Source(Vertex *now, int cnt) : now(now), cnt(cnt) {}
        void pass(Vertex &v, int i) {
            CHECK(cnt > 0) << "vertex exceeded!";
            *now++ = v;
            --cnt;
        }
    };

    class Generator : public Base {
        int cnt;
    public:
        Generator(int cnt) : cnt(cnt) {}
        void generate() {
            for (int i = 0; i < cnt; ++i) {
                Vertex v;
                emit(v, i);
            }
        }
        void pass(Vertex &v, int i) {
            for (int j = 0; j < cnt; ++j) {
                emit(v, j);
            }
        }
    };

    class Emitter : public Base {
        float start, interval;
    public:
        Emitter(float start, float interval)
            : start(start), interval(interval) {}
        void pass(Vertex &v, int i) {
            float t = start + interval * i;
            v.time = glm::vec4(t, INFINITY, t, INFINITY);
            emit(v, i);
        }
    };

    class Point : public Base {
        glm::vec3 pos;
    public:
        Point(const glm::vec3 &pos) : pos(pos) {}
        void pass(Vertex &v, int i) {
            v.position = pos;
            emit(v, i);
        }
    };

    class Direction : public Base {
        glm::vec3 dir;
    public:
        Direction(const glm::vec3 &dir) : dir(dir) {}
        void pass(Vertex &v, int i) {
            v.velocity = dir;
            v.acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
            emit(v, i);
        }
    };

    class Type : public Base {
        static const int maxSize = 32;
        glm::vec4 uv;
    public:
        Type(int type, int color, int scale, float size) {
            CHECK(type >= 0 && type + scale <= maxSize
                  && color >= 0 && color + scale <= maxSize)
                << "danmaku type not supported!";

            uv = glm::vec4((float)color / 32.0f, (float)type / 32.0f,
                           (float)scale / 32.0f, size);
        }
        void pass(Vertex &v, int i) {
            v.uvIndex = uv;
            emit(v, i);
        }
    };

    class DieAfter : public Base {
        float delta;
    public:
        DieAfter(float delta) : delta(delta) {}
        void pass(Vertex &v, int i) {
            v.time[1] = v.time[3] = v.time[0] + delta;
            emit(v, i);
        }
    };

    Base *source(Vertex *now, int cnt) { return new Source(now, cnt); }
    Base *generator(int cnt) { return new Generator(cnt); }
    Base *emitter(float start, float interval) {
        return new Emitter(start, interval);
    }
    Base *point(const glm::vec3 &pos) { return new Point(pos); }
    Base *direction(const glm::vec3 &dir) { return new Direction(dir); }
    Base *type(int type, int color, int scale, float size) {
        return new Type(type, color, scale, size);
    }
    Base *dieAfter(float delta) { return new DieAfter(delta); }
}
