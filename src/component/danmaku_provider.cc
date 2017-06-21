#include "danmaku_provider.hpp"

void DanmakuProvider::reset() { pool.clear(); }
size_t DanmakuProvider::size() const { return sizeof(Vertex) * pool.size(); }
const void *DanmakuProvider::data() const { return pool.data(); }
