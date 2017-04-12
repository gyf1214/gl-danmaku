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

    class Sphere : public Base {
        int split, height;
    public:
        Sphere(int split, int height) : split(split), height(height) {}
        void pass(Vertex &v, int t) {
            for (int i = -height; i <= height; ++i) {
                float a0 = (float)i / (float)(height + 1) * M_PI / 2.0f;
                int k = 4 * round(1.0f + (float)split * cos(a0));
                float a1 = 2.0f * M_PI / (float)k;
                for (int j = 0; j < k; ++j) {
                    float a2 = a1 * (float)j;
                    v.velocity = glm::vec3(
                        cos(a0) * cos(a2),
                        cos(a0) * sin(a2),
                        sin(a0)
                    );
                    emit(v, t);
                }
            }
        }
    };

    class CircleMotion : public Base {
        glm::vec3 center;
    public:
        CircleMotion(const glm::vec3 &center) : center(center) {}
        void pass(Vertex &v, int i) {
            float r = length(v.position - center);
            float a = dot(v.velocity, v.velocity) / r;
            v.acceleration = glm::vec4(center, a);
            emit(v, i);
        }
    };

    class CrossBias : public Base {
        glm::vec3 base;
    public:
        CrossBias(const glm::vec3 &base) : base(base) {}
        void pass(Vertex &v, int i) {
            v.acceleration = glm::vec4(cross(base, v.velocity), 0.0f);
            emit(v, i);
        }
    };

    class Circle : public Base {
        glm::vec3 x, y;
        float angle;
    public:
        Circle(const glm::vec3 &x, const glm::vec3 &y, float angle)
            : x(x), y(y), angle(angle) {}
        void pass(Vertex &v, int i) {
            v.velocity = x * (float)cos(angle * i) + y * (float)sin(angle * i);
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
    Base *linearSpeed(float base, float k) { return new LinearSpeed(base, k); }
    Base *sphere(int split, int height) { return new Sphere(split, height); }
    Base *circleMotion(const glm::vec3 &center) {
        return new CircleMotion(center);
    }
    Base *crossBias(const glm::vec3 &base) { return new CrossBias(base); }
    Base *circle(const glm::vec3 &x, const glm::vec3 &y, float angle) {
        return new Circle(x, y, angle);
    }
}
