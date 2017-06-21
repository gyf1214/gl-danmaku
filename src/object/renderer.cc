#include "ext.hpp"

using namespace std;

class BasicRenderer : public Renderer {
    vector<Object *> objects;
public:
    void setup() { for (const auto &o : objects) o->setup(); }
    void reset() { for (const auto &o : objects) Box::release(o); objects.clear(); }
    void render() { for (const auto &o : objects) o->render(); }
    void push(Object *o) { objects.push_back(o); }
};
