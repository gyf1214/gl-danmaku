#ifndef __CORE_FIBER
#define __CORE_FIBER

#include "../common.hpp"

class Fiber {
public:
    typedef void (*Runnable)(void *);
    virtual ~Fiber();
    virtual bool resume(void) = 0;

    static Fiber *create(Runnable r, void *arg);
    static void yield(void);
};

#endif
