#ifndef __CORE_BOX
#define __CORE_BOX

#include "object.hpp"

class Box {
protected:
    static void *alloc(size_t n);
    static void dealloc(void *o);

    template <typename U, typename ...Ts>
    static U *create(Ts ...args) {
        void *ret = alloc(sizeof(U));
        return new(ret) U(args...);
    }
public:
    static void release(BaseObject *o) {
        if (o) {
            o->reset();
            o->~BaseObject();
            dealloc(o);
        }
    }
};

#endif
