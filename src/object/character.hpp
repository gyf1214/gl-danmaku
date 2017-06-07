#ifndef __CHARACTER
#define __CHARACTER

#include "program_renderer.hpp"
#include "mmd-physics/motion.hpp"

using mmd::pmx::Vertex;

proto(Character, Shader::character);

protoBuffer = {
    { 0, NULL },
    { 0, NULL },
    { 0, NULL },
};

protoAttrib = {
    { "position"  , Offset(Vertex, position[0])   , 3, sizeof(Vertex) },
    { "normal"    , Offset(Vertex, normal[0])     , 3, sizeof(Vertex) },
    { "uv"        , Offset(Vertex, uv[0])         , 2, sizeof(Vertex) },
    { "boneIndex" , Offset(Vertex, BDEF.bone[0])  , 4, sizeof(Vertex), true, GL_INT },
    { "boneWeight", Offset(Vertex, BDEF.weight[0]), 4, sizeof(Vertex) }
};

protoUnifom = {
    "mMat", "vMat", "pMat",
    "lightPosition", "lightColor", "ambient", "lightMaterial",
    "diffuse", "specular", "texture0", "side",
    "morphData", "morphCount", "morphs"
};

class Character : public ProgramRenderer<Proto> {
protected:
    static const int uboBinding = 1;

    mmd::physics::Motion *motion;
    Model *model;
    mmd::vmd::Motion *vMotion;

    std::vector<glm::mat4> bones;
    std::vector<float> morphs;

    void setupBuffers(void);
public:
    Character(Scene *scene, Model *model, mmd::vmd::Motion *vMotion);
    ~Character();
    void setup(void);
    void render(void);
    void update(void);
    void reset(void);
};

#endif
