#include "common.hpp"

namespace Builder {
    class RandomDirection : public Base {
    public:
        void pass(Vertex &v, int i) {
            v.velocity = glm::sphericalRand(1.0f);
            emit(v, i);
        }
    };

    class RandomBox : public Base {
        vec3 min, max;
    public:
        RandomBox(const vec3 &min, const vec3 &max) : min(min), max(max) {}
        void pass(Vertex &v, int i) {
            v.position = glm::linearRand(min, max);
            emit(v, i);
        }
    };

    Base *randomDirection() {
        return new RandomDirection();
    }

    Base *randomBox(const vec3 &min, const vec3 &max) {
        return new RandomBox(min, max);
    }
} /* Builder */
