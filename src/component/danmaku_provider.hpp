#ifndef __COM_DANMAKU_PROVIDER
#define __COM_DANMAKU_PROVIDER

#include "../ext.hpp"
#include "component/provider.hpp"
#include "vertex/danmaku.hpp"

class DanmakuProvider : public virtual Provider {
protected:
    std::vector<Vertex> pool;
public:
    void reset();
    size_t size() const;
    const void *data() const;
};

#endif
