#include "builder.hpp"
#include "util.hpp"

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

    void Chain::operator <<(const Emit &e) const {
        start -> generate();
    }

    class MultiBase : public Base {
        std::vector<Base *>base;
    public:
        ~MultiBase() { for (auto x : base) { x -> reset(); } }

        void push(Base *b) { base.push_back(b -> set()); }
        void pass(Vertex &v, int i) { for (auto x : base) x -> pass(v, i); }
        void chain(Base *b) { for (auto x : base) x -> chain(b); }
    };

    Multi::Multi() : base(new MultiBase()) { base -> set(); }
    Multi::Multi(const Multi &m) : base(m.base) { base -> set(); }
    Multi::~Multi() { base -> reset(); }

    const Multi &Multi::operator <<(Base *b) const {
        base -> push(b);
        return *this;
    }

    Multi::operator Base *() const {
        return base;
    }
}
