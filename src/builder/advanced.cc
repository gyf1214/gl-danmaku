#include "builder.hpp"

namespace Builder {
    class Target : public Base {
        glm::vec3 pos;
        float f;
        bool norm;
    public:
        Target(const glm::vec3 &pos, float f, bool norm)
            : pos(pos), f(f), norm(norm) {}
        void pass(Vertex &v, int i) {
            v.velocity = pos - v.position;
            if (norm) {
                v.velocity = normalize(v.velocity);
            }
            v.velocity *= f;
            emit(v, i);
        }
    };

    class Line : public Base {
        glm::vec3 pos, dir;
    public:
        Line(const glm::vec3 &pos, const glm::vec3 &dir)
            : pos(pos), dir(dir) {}
        void pass(Vertex &v, int i) {
            v.position = pos + dir * (float)i;
            emit(v, i);
        }
    };

    class LinearSpeed : public Base {
        float base, k;
    public:
        LinearSpeed(float base, float k) : base(base), k(k) {}
        void pass(Vertex &v, int i) {
            float speed = base + k  * (float)i;
            v.velocity = normalize(v.velocity) * speed;
            emit(v, i);
        }
    };

    Base *targetTime(const glm::vec3 &pos, float time) {
        return new Target(pos, 1.0f / time, false);
    }
    Base *targetNorm(const glm::vec3 &pos, float speed) {
        return new Target(pos, speed, true);
    }
    Base *line(const glm::vec3 &pos, const glm::vec3 &dir) {
        return new Line(pos, dir);
    }
    Base *linearSpeed(float base, float k) {
        return new LinearSpeed(base, k);
    }
}
