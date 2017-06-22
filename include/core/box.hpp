#ifndef __CORE_BOX
#define __CORE_BOX

#include "object.hpp"

class Box {
protected:
    static void *alloc(size_t n);
    static void dealloc(void *o);
    static void track(BaseObject *o);
public:
    template <typename U, typename ...Ts>
    static U *create(Ts ...args) {
        void *ret = alloc(sizeof(U));
        return new(ret) U(args...);
    }
    template <typename U, typename ...Ts>
    static U *global(Ts ...args) {
        U *ret = create<U>(args...);
        track(ret);
        return ret;
    }
    static void release(BaseObject *o);
    static void releaseGlobal(void);
};

#endif
