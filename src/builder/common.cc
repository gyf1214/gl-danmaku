#include "builder.hpp"

namespace Builder {
    Base::Base() : refcnt(0), next(NULL) {}
    Base::Base(const Base &b) : refcnt(0), next(NULL) {}
    Base::~Base() { if (next) next -> reset(); }
    Base *Base::set() { ++refcnt; return this; }
    void Base::reset() { if (--refcnt <= 0) delete this; }
    void Base::generate(void) {}
    void Base::pass(const Vertex &v) {}
    void Base::emit(const Vertex &v) { next -> pass(v); }
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
}
