#include "ext.hpp"
#include "fiber.hpp"

using namespace std;

class FiberExt : public Fiber {
    thread pthread;
    mutex resumeSign, yieldSign;
    bool end;

    struct Worker {
        Runnable r;
        void *arg;
        FiberExt *self;

        Worker(Runnable r, void *arg, FiberExt *self)
            : r(r), arg(arg), self(self) {}

        void operator ()() {
            self->resumeSign.lock();
            r(arg);
            self->end = true;
        }
    } worker;
public:
    static FiberExt *current;

    FiberExt(Runnable r, void *arg) : end(false), worker(r, arg, this)  {
        resumeSign.lock();
        yieldSign.lock();
        pthread = thread(worker);
    }

    ~FiberExt() {
        resumeSign.unlock();
        yieldSign.unlock();
        pthread.join();
    }

    bool resume() {
        CHECK(!end) << "call to a terminated fiber!";
        current = this;
        resumeSign.unlock();
        yieldSign.lock();
        return end;
    }

    static void yield() {
        FiberExt *last = current;
        CHECK_NQ(last, NULL) << "yield from main thread!";
        current = NULL;
        last->yieldSign.unlock();
        last->resumeSign.lock();
    }
};

FiberExt *FiberExt::current = NULL;

Fiber::~Fiber() {}

Fiber *Fiber::create(Runnable r, void *arg) {
    return new FiberExt(r, arg);
}

void Fiber::yield() {
    FiberExt::yield();
}
