#ifndef __COM_CAMERA
#define __COM_CAMERA

#include "transform.hpp"

class Camera : public virtual Translate {
public:
    virtual vec3 direction(void) = 0;
    virtual float fovy(void) = 0;
    virtual glm::mat4 vMat(void) = 0;
    virtual glm::mat4 pMat(void) = 0;
};

#endif
