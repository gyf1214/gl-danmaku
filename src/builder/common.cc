#include "common.hpp"

namespace Builder {
    Base::Base() : refcnt(0), next(NULL) {}
    Base::Base(const Base &b) : refcnt(0), next(NULL) {}
    Base::~Base() { if (next) next -> reset(); }
    Base &Base::operator =(const Base &b) { return *this; }
    Base *Base::set() { ++refcnt; return this; }
    void Base::reset() { if (--refcnt <= 0) delete this; }
    void Base::generate(void) {
        Vertex v;
        v.velocity = v.position = vec3(0.0f);
        v.acceleration = vec4(0.0f);
        v.time = vec4(0.0f, INFINITY, 0.0f, INFINITY);
        v.fade = vec3(0.0f, 1000.0f, 1000.0f);
        pass(v, 0);
    }
    void Base::emit(const Vertex &v, int i) { next -> pass(v, i); }
    void Base::chain(Base *b) { next = b -> set(); }

    Emit::Emit(Base *base) : base(base->set()) {}
    Emit::Emit(const Emit &e) : base(e.base->set()) {}
    Emit::~Emit() { base->reset(); }
    Emit &Emit::operator=(const Emit &c) {
        base->reset();
        base = c.base->set();
        return *this;
    }
    Base *Emit::source() const { return base; }

    Chain::Chain(Base *x) : start(x -> set()), end(x -> set()) {}
    Chain::Chain(Base *s, Base *e)
        : start(s -> set()), end(e -> set()) {}
    Chain::Chain(const Chain &c)
        : start(c.start -> set()), end(c.end -> set()) {}
    Chain::~Chain() { start -> reset(); end -> reset(); }

    Chain &Chain::operator =(const Chain &c) {
        start->reset();
        end->reset();
        start = c.start->set();
        end = c.end->set();
        return *this;
    }

    Chain Chain::operator <<(Base *b) const {
        end -> chain(b);
        return Chain(start, b);
    }

    Chain Chain::operator <<(const Chain &c) const {
        end -> chain(c.start);
        return Chain(start, c.end);
    }

    void Chain::operator <<(const Emit &e) const {
        end->chain(e.source());
        start -> generate();
    }

    void Chain::emit(Vertex &v, int i) const {
        start -> pass(v, i);
    }

    void Chain::generate() const {
        start -> generate();
    }

    class MultiBase : public Base {
        std::vector<Chain>base;
    public:
        void push(const Chain &c) { base.push_back(c); }
        void push(Base *b) { base.push_back(Chain(b)); }
        void pass(Vertex v, int i) { for (const auto &x : base) x.emit(v, i); }
        void generate() { for (const auto &x : base) x.generate(); }
        void chain(Base *b) { for (const auto &x : base) x << b; }
    };

    Multi::Multi() : base(new MultiBase()) { base -> set(); }
    Multi::Multi(const Multi &m) : base(m.base) { base -> set(); }
    Multi::~Multi() { base -> reset(); }

    Multi &Multi::operator =(const Multi &m) {
        base->reset();
        base = m.base;
        base->set();
        return *this;
    }

    const Multi &Multi::operator <<(Base *b) const {
        base -> push(b);
        return *this;
    }

    const Multi &Multi::operator <<(const Chain &b) const {
        base -> push(b);
        return *this;
    }

    Multi::operator Base *() const {
        return base;
    }
}
