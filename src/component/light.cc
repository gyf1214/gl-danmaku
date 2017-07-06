#include "../ext.hpp"
#include "component/light.hpp"

using namespace std;

class BasicLight : public virtual LightManager {
    vector<Light> lights;
    vector<vec2> fades;
    int now;
public:
    void setup() { LOG << "setup basic light"; }
    void reset() { LOG << "reset basic light"; lights.clear(); fades.clear(); }

    void update() {
        int n = lights.size();
        for (int i = 0; i < n; ++i) {
            if (fades[i].y != 0.0f) {
                fades[i].x += fades[i].y * Application::elapse();
                if (fades[i].x >= 1.0f) {
                    fades[i].x = 1.0f;
                    fades[i].y = 0.0f;
                } else if (fades[i].x <= 0.0f) {
                    fades.erase(fades.begin() + i);
                    lights.erase(lights.begin() + i);
                    --i;
                    --n;
                }
            }
        }
    }

    Light light(int i) const {
        Light l = lights[i];
        l.color *= fades[i].x;
        l.ambient *= fades[i].x;
        return l;
    }

    Light &light(int i) { return lights[i]; }

    int count() const { return lights.size(); }
    int pass() const { return now; }
    int &pass() { return now; }

    void remove(int i, float fade = 0.0f) {
        if (fade > 0.0f) {
            fades[i].y = -1.0f / fade;
        } else {
            lights.erase(lights.begin() + i);
            fades.erase(fades.begin() + i);
        }
    }

    void push(const Light &l, float fade = 0.0f) {
        lights.push_back(l);
        if (fade > 0.0f) {
            fades.push_back(vec2(0.0f, 1.0f / fade));
        } else {
            fades.push_back(vec2(1.0f, 0.0f));
        }
    }

    void ambient(vec3 color, float fade = 0.0f) {
        push(Light(vec4(0.0f, 0.0f, 0.0f, 1.0f), vec3(0.0f),
                   color, vec4(1.0f, 0.0f, 0.0f, 1.0f)), fade);
    }
    void point(vec3 pos, vec3 color, float radius, float alpha, float fade = 0.0f) {
        push(Light(vec4(pos, 1.0f), color, vec3(0.0f),
                   vec4(alpha, 0.0f, (1 - alpha) / radius / radius, 0.0f)), fade);
    }
    void parallel(vec3 dir, vec3 color, float fade = 0.0f) {
        push(Light(vec4(normalize(dir), 0.0f), color, vec3(0.0f), vec4(0.0f)), fade);
    }
};

LightManager *LightManager::basic() {
    return Box::create<BasicLight>();
}
