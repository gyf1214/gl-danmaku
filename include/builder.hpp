#ifndef __DANMAKU_BUILDER
#define __DANMAKU_BUILDER

#include <vector>
#include "vertex.hpp"
#include "util.hpp"

namespace Builder {
    class Base {
        int refcnt;
        Base *next;
    public:
        Base();
        Base(const Base &c);
        virtual ~Base();

        Base *set();
        void reset();

        virtual void generate(void);
        virtual void pass(const Vertex &v);
        virtual void emit(const Vertex &v);
        virtual void chain(Base *b);
    };

    struct Emit {};

    class Chain {
        Base *start, *end;
    public:
        Chain(Base *x);
        Chain(Base *s, Base *e);
        Chain(const Chain &c);
        ~Chain();

        Chain operator <<(Base *b) const;
        void operator <<(const Emit &e) const;
    };

    class Multi : public Base {
        std::vector<Base *>base;
    public:

    };

    // Source
    Base *source(Vertex *vertex, int cnt);
    Base *type(int type, int color);

    // Generators
    Base *generator(float start, float interval, int cnt);

    // Position Pass
    Base *point(const glm::vec3 &pos);
    inline Base *point(float x, float y, float z) {
        return point(glm::vec3(x, y, z));
    }

    // Speed Pass
    Base *direction(const glm::vec3 &dir);
    inline Base *direction(float x, float y, float z) {
        return direction(glm::vec3(x, y, z));
    }
}

#endif
