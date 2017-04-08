#ifndef __DANMAKU_BUILDER
#define __DANMAKU_BUILDER

#include <vector>
#include "vertex/danmaku.hpp"
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
        virtual void pass(Vertex &v, int i);
        virtual void emit(Vertex &v, int i);
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
        Chain operator <<(const Chain &c) const;
        void operator <<(const Emit &e) const;
    };

    class MultiBase;
    class Multi {
        MultiBase *base;
    public:
        Multi();
        Multi(const Multi &);
        ~Multi();

        const Multi &operator <<(Base *b) const;
        operator Base *() const;
    };

    // Misc
    Base *source(Vertex *vertex, int cnt);
    Base *type(int type, int color, int scale, float size);

    // Generators
    Base *generator(int cnt);

    // Time Pass
    Base *emitter(float start, float interval);
    Base *dieAfter(float delta);

    // Position Pass
    Base *point(const glm::vec3 &pos);
    Base *line(const glm::vec3 &start, const glm::vec3 &dir);

    // Motion Pass
    Base *direction(const glm::vec3 &dir);
    Base *targetTime(const glm::vec3 &pos, float time);
    Base *targetNorm(const glm::vec3 &pos, float speed);
    Base *linearSpeed(float base, float k);
    Base *sphere(int split, int height);

#define inlineVec3(name) inline Base *name(float x, float y, float z) {\
    return name(glm::vec3(x, y, z));\
}

    inlineVec3(point);
    inlineVec3(direction);
}

#endif
