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
            LOG << "fiber " << self->pthread.get_id() << " start";
            r(arg);
            self->end = true;
            Fiber::yield();
            LOG << "fiber " << self->pthread.get_id() << " terminated";
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
        FiberExt *last = current;
        current = this;
        resumeSign.unlock();
        yieldSign.lock();
        LOG << "fiber " << pthread.get_id() << " yield";
        current = last;
        return end;
    }

    static void yield() {
        FiberExt *last = current;
        CHECK_NQ(last, NULL) << "yield from main thread!";
        last->yieldSign.unlock();
        last->resumeSign.lock();
        LOG << "fiber " << last->pthread.get_id() << " resume from another";
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
