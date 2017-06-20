#ifndef __COM_CAMERA
#define __COM_CAMERA

#include "transform.hpp"

class Camera : public virtual Translate {
public:
    virtual vec3 direction(void) const = 0;
    virtual float fovy(void) const = 0;
    virtual glm::mat4 vMat(void) const = 0;
    virtual glm::mat4 pMat(void) const = 0;

    // Factories
    static Camera *free(vec3 pos, float horizon, float vertical, float fov);
};

#endif
