#ifndef __COM_DANMAKU
#define __COM_DANMAKU

#include "../core.hpp"

class Provider : public virtual BaseObject {
public:
    virtual const void *data(void) const = 0;
    virtual size_t size(void) const = 0;
};

#endif
