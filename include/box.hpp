#ifndef __BOX
#define __BOX

#include "common.hpp"

template <typename T>
class Box {
protected:
    template <typename U>
    static U *alloc() {
        return (U *)malloc(sizeof(U));
    }

    template <typename U, typename ...Ts>
    static U *create(Ts ...args) {
        return new U(args...);
    }
public:
    static void release(T *o) {
        if (o) {
            o -> reset();
            delete o;
        }
    }
};

#endif
