#ifndef __SCENE_BOX
#define __SCENE_BOX

#include "box.hpp"

class SceneBox : public Box<Scene> {
public:
    static Scene *scene1(void);
};

#endif
