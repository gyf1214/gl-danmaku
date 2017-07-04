#include "common.hpp"

namespace Builder {
    class RandomDirection : public Base {
    public:
        void pass(Vertex &v, int i) {
            v.velocity = glm::sphericalRand(1.0f);
            emit(v, i);
        }
    };

    Base *randomDirection() {
        return new RandomDirection();
    }
} /* Builder */
