#ifndef __BOX
#define __BOX

#include <cstdlib>

template <typename T>
class Box {
protected:
    template <typename U>
    static T *alloc() {
        return (T *)malloc(sizeof(U));
    }

    template <typename U, typename ...Ts>
    static T *create(Ts ...args) {
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
