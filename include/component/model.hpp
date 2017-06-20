#ifndef __COM_MODEL
#define __COM_MODEL

#include "../core.hpp"

class Model : public virtual Component {
public:
    virtual GLuint texture(int index) const = 0;
    virtual GLuint morphTexture(void) const = 0;
    virtual const MMDModel *mmdModel(void) const = 0;
};

#endif
