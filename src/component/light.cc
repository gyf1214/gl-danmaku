#include "../ext.hpp"
#include "component/light.hpp"

using namespace std;

class BasicLight : public virtual LightManager {
    vector<Light> lights;
    int now;
public:
    void setup() { LOG << "setup basic light"; }
    void reset() { LOG << "reset basic light"; lights.clear(); }
    void update() {}

    const Light &light(int i) const { return lights[i]; }
    Light &light(int i) { return lights[i]; }
    int count() const { return lights.size(); }
    int pass() const { return now; }
    int &pass() { return now; }

    void remove(int i) { lights.erase(lights.begin() + i); }
    void push(const Light &l) { lights.push_back(l); }
    void ambient(vec3 color) {
        push(Light(vec4(0.0f, 0.0f, 0.0f, 1.0f), vec3(0.0f),
                   color, vec4(1.0f, 0.0f, 0.0f, 1.0f)));
    }
    void point(vec3 pos, vec3 color, float radius, float alpha) {
        push(Light(vec4(pos, 1.0f), color, vec3(0.0f),
                   vec4(alpha, (1.0f - alpha) / radius, 0.0f, 0.0f)));
    }
    void parallel(vec3 dir, vec3 color) {
        push(Light(vec4(normalize(dir), 0.0f), color, vec3(0.0f), vec4(0.0f)));
    }
};

LightManager *LightManager::basic() {
    return Box::create<BasicLight>();
}
