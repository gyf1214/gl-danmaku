#ifndef __DANMAKU_BUILDER
#define __DANMAKU_BUILDER

#include "../ext.hpp"
#include "vertex/danmaku.hpp"

namespace Builder {
    class Base {
        int refcnt;
        Base *next;
    public:
        Base();
        Base(const Base &c);
        virtual ~Base();

        Base &operator =(const Base &b);

        Base *set();
        void reset();

        virtual void generate(void);
        virtual void pass(Vertex v, int i) = 0;
        virtual void emit(const Vertex &v, int i);
        virtual void chain(Base *b);
    };

    class Emit {
        Base *base;
    public:
        Emit(Base *base);
        Emit(const Emit &e);
        ~Emit();

        Emit &operator =(const Emit &c);

        Base *source(void) const;
    };

    class Chain {
        Base *start, *end;
    public:
        Chain(Base *x);
        Chain(Base *s, Base *e);
        Chain(const Chain &c);
        ~Chain();

        Chain &operator =(const Chain &c);

        void emit(Vertex &v, int i) const;
        void generate(void) const;
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

        Multi &operator =(const Multi &);

        const Multi &operator <<(Base *b) const;
        const Multi &operator <<(const Chain &m) const;
        operator Base *() const;
    };

    // Misc
    Base *source(std::vector<Vertex> &pool);
    Base *type(int type, int color, int scale, float size);
    Base *fadeIn(float time);
    Base *fadeOut(float time);

    // Generators
    Base *generator(int cnt);

    // Time Pass
    Base *emitter(float start, float interval);
    Base *dieAfter(float delta);

    // Position Pass
    Base *point(const vec3 &pos);
    Base *line(const vec3 &start, const vec3 &dir);
    Base *circle(const vec3 &x, const vec3 &y, float angle);
    Base *randomBox(const vec3 &min, const vec3 &max);

    // Motion Pass
    Base *direction(const vec3 &dir);
    Base *targetTime(const vec3 &pos, float time);
    Base *targetNorm(const vec3 &pos, float speed);
    Base *linearSpeed(float base, float k);
    Base *sphereSpread(int split, int height);
    Base *circleSpread(const vec3 &x, const vec3 &y, float angle);
    Base *circleMotion(const vec3 &center);
    Base *crossBias(const vec3 &base);
    Base *randomDirection(void);

    // Additive
    Base *addPosition(const vec3 &p);
    Base *addVelocity(const vec3 &v);
    Base *addTime(float t);

#define inlineVec3(name) inline Base *name(float x, float y, float z) {\
    return name(vec3(x, y, z));\
}

    inlineVec3(point);
    inlineVec3(direction);
    inlineVec3(circleMotion);
    inlineVec3(crossBias);
    inlineVec3(addPosition);
    inlineVec3(addVelocity);
}

#endif
