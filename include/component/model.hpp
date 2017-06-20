#ifndef __COM_MODEL
#define __COM_MODEL

#include "../core.hpp"

class Model : public virtual BaseObject {
public:
    virtual GLuint texture(int index) const = 0;
    virtual GLuint morphTexture(void) const = 0;
    virtual const MMDModel *mmdModel(void) const = 0;

    // Factories
    static Model *reimu(void);
    static Model *utsuho(void);
    static Model *suwako(void);

    // Motions
    static MMDMotion *test1();
    static MMDMotion *test2();
};

#endif
