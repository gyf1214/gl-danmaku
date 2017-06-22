#include "common.hpp"

namespace Builder {
    Base::Base() : refcnt(0), next(NULL) {}
    Base::Base(const Base &b) : refcnt(0), next(NULL) {}
    Base::~Base() { if (next) next -> reset(); }
    Base *Base::set() { ++refcnt; return this; }
    void Base::reset() { if (--refcnt <= 0) delete this; }
    void Base::generate(void) {}
    void Base::pass(Vertex &v, int i) {}
    void Base::emit(Vertex &v, int i) { next -> pass(v, i); }
    void Base::chain(Base *b) { next = b -> set(); }

    Chain::Chain(Base *x) : start(x -> set()), end(x -> set()) {}
    Chain::Chain(Base *s, Base *e)
        : start(s -> set()), end(e -> set()) {}
    Chain::Chain(const Chain &c)
        : start(c.start -> set()), end(c.end -> set()) {}
    Chain::~Chain() { start -> reset(); end -> reset(); }

    Chain Chain::operator <<(Base *b) const {
        end -> chain(b);
        return Chain(start, b);
    }

    Chain Chain::operator <<(const Chain &c) const {
        end -> chain(c.start);
        return Chain(start, c.end);
    }

    void Chain::operator <<(const Emit &e) const {
        start -> generate();
    }

    void Chain::emit(Vertex &v, int i) const {
        start -> pass(v, i);
    }

    class MultiBase : public Base {
        std::vector<Chain>base;
    public:
        void push(const Chain &c) { base.push_back(c); }
        void push(Base *b) { base.push_back(Chain(b)); }
        void pass(Vertex &v, int i) { for (const auto &x : base) x.emit(v, i); }
        void chain(Base *b) { for (const auto &x : base) x << b; }
    };

    Multi::Multi() : base(new MultiBase()) { base -> set(); }
    Multi::Multi(const Multi &m) : base(m.base) { base -> set(); }
    Multi::~Multi() { base -> reset(); }

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
