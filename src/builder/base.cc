#include "builder.hpp"
#include "util.hpp"

namespace Builder {
    class Source : public Base {
        Vertex *now;
        int cnt;
    public:
        Source(Vertex *now, int cnt) : now(now), cnt(cnt) {}
        void pass(const Vertex &v) {
            CHECK(cnt > 0) << "vertex exceeded!";
            *now++ = v;
            --cnt;
        }
    };

    Base *source(Vertex *now, int cnt) { return new Source(now, cnt); }

    class Generator : public Base {
        float start, interval;
        int cnt;
    public:
        Generator(float start, float interval, float cnt)
            : start(start), interval(interval), cnt(cnt) {}
        void generate() {
            float now = start;
            for (int i = 0; i < cnt; ++i) {
                Vertex v;
                v.time = glm::vec4(now, INFINITY, now, INFINITY);
                emit(v);
                now += interval;
            }
        }
    };

    Base *generator(float start, float interval, int cnt) {
        return new Generator(start, interval, cnt);
    }

    class Point : public Base {
        glm::vec3 pos;
    public:
        Point(const glm::vec3 &pos) : pos(pos) {}
        void pass(const Vertex &v) {
            Vertex vv = v;
            vv.position = pos;
            emit(vv);
        }
    };

    Base *point(const glm::vec3 &pos) { return new Point(pos); }


    class Direction : public Base {
        glm::vec3 dir;
    public:
        Direction(const glm::vec3 &dir) : dir(dir) {}
        void pass(const Vertex &v) {
            Vertex vv = v;
            vv.velocity = dir;
            vv.acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
            emit(vv);
        }
    };

    Base *direction(const glm::vec3 &dir) { return new Direction(dir); }

    class Type : public Base {
        glm::vec4 uv;
    public:
        Type(int type, int color) {
            CHECK(type >= 4 && type < 16 && color >= 0 && color < 16)
                << "danmaku type not supported!";

            uv = glm::vec4((float)color / 16.0f, (float)type / 16.0f,
                           1.0f / 16.0f, 1.0f / 16.0f);
        }
        void pass(const Vertex &v) {
            Vertex vv = v;
            vv.uvIndex = uv;
            emit(vv);
        }
    };

    Base *type(int type, int color) { return new Type(type, color); }
}
