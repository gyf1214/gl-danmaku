#ifndef __DANMAKU_BUILDER
#define __DANMAKU_BUILDER

#include "vertex.hpp"

class DanmakuBuilder {
public:
    DanmakuBuilder &type(const glm::vec3 &uv);
    DanmakuBuilder &setTime(double newTime);
};

class BuilderBase {
protected:
public:
};

#endif
