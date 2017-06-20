#include "ext.hpp"
#include "component/character.hpp"

using namespace glm;
using namespace std;
using namespace mmd;

using mmd::pmx::Vertex;

proto(Character, Shader::character);

protoBuffer(Character) = {
    { 0, NULL },
    { 0, NULL },
    { 0, NULL },
};

protoAttrib(Character) = {
    { "position"  , Offset(Vertex, position[0])   , 3, sizeof(Vertex) },
    { "normal"    , Offset(Vertex, normal[0])     , 3, sizeof(Vertex) },
    { "uv"        , Offset(Vertex, uv[0])         , 2, sizeof(Vertex) },
    { "boneIndex" , Offset(Vertex, BDEF.bone[0])  , 4, sizeof(Vertex), true, GL_INT },
    { "boneWeight", Offset(Vertex, BDEF.weight[0]), 4, sizeof(Vertex) }
};

protoUniform(Character) = {
    "mMat", "vMat", "pMat",
    "lightPosition", "lightColor", "ambient", "lightMaterial",
    "diffuse", "specular", "texture0", "side",
    "morphData", "morphCount", "morphs"
};

protoTexture(Character) = {};

class CharacterObject : public ProgramBase<CharacterProto>, public virtual Object {
protected:
    static const int uboBinding = 1;

    Character *character;
    Camera *camera;
    LightManager *lights;

    const Model *model;
    const MMDModel *mModel;
    const vector<mat4> *skins;
    const vector<float> *faces;

    void setupBuffers() {
        int v = mModel->mesh.vertex.size();
        int s = mModel->mesh.surface.size();

        glGenBuffers(BufferSize, buffer);
        LOG << "generate " << BufferSize << " buffers";

        glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
        glBufferData(GL_ARRAY_BUFFER, v * sizeof(Vertex),
            &mModel->mesh.vertex[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, s * sizeof(mmd::pmx::Surface),
            &mModel->mesh.surface[0], GL_STATIC_DRAW);

        glBindBuffer(GL_UNIFORM_BUFFER, buffer[2]);
        glBufferData(GL_UNIFORM_BUFFER, skins->size() * sizeof(mat4),
                     NULL, GL_DYNAMIC_DRAW);
    }
public:
    CharacterObject(Character *ch, Camera *cam, LightManager *lights)
        : character(ch), camera(cam), lights(lights) {}

    void setup() {
        model = character->model();
        mModel = model->mmdModel();
        skins = &character->skins();
        faces = &character->faces();

        ProgramBase::setup();

        bindBuffer(buffer[0]);

        GLuint uboIndex = glGetUniformBlockIndex(program, "Bones");
        glUniformBlockBinding(program, uboIndex, uboBinding);

        glUniform1i(uniform[9], 0);
        glUniform1i(uniform[11], 1);
        glUniform1i(uniform[12], faces->size());
    }

    void render() {
        bindProgram();

        glEnable(GL_CULL_FACE);

        glUniformMatrix4fv(uniform[0], 1, GL_FALSE, &character->matrix()[0][0]);
        glUniformMatrix4fv(uniform[1], 1, GL_FALSE, &camera->vMat()[0][0]);
        glUniformMatrix4fv(uniform[2], 1, GL_FALSE, &camera->pMat()[0][0]);

        Light light = lights->current();
        bool ambient = !lights->pass();

        glUniform3fv(uniform[5], 1, &light.ambient[0]);
        glUniform4fv(uniform[3], 1, &light.position[0]);
        glUniform3fv(uniform[4], 1, &light.color[0]);
        glUniform4fv(uniform[6], 1, &light.material[0]);
        glUniform1fv(uniform[13], faces->size(), &(*faces)[0]);

        glBindBuffer(GL_UNIFORM_BUFFER, buffer[2]);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, skins->size() * sizeof(mat4),
                        &(*skins)[0][0][0]);
        glBindBufferBase(GL_UNIFORM_BUFFER, uboBinding, buffer[2]);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_BUFFER, model->morphTexture());

        glActiveTexture(GL_TEXTURE0);

        int sum = 0;

        for (int i = 0; i < mModel->materials.size(); ++i) {
            const auto &material = mModel->materials[i];

            if (ambient) glUniform3fv(uniform[5], 1, &material.ambient[0]);
            glUniform4fv(uniform[7], 1, &material.diffuse[0]);
            glUniform4fv(uniform[8], 1, &material.specular[0]);

            glBindTexture(GL_TEXTURE_2D, model->texture(material.texture));

            glUniform1f(uniform[10], 1.0f);
            glCullFace(GL_FRONT);
            glDrawElements(GL_TRIANGLES, material.count,
                GL_UNSIGNED_INT, (void *)(sum * sizeof(GLuint)));

            if (material.flag & mmd::pmx::Material::DoubleSided) {
                glUniform1f(uniform[10], -1.0f);
                glCullFace(GL_BACK);
                glDrawElements(GL_TRIANGLES, material.count,
                    GL_UNSIGNED_INT, (void *)(sum * sizeof(GLuint)));
            }

            sum += material.count;
        }

        glDisable(GL_CULL_FACE);
    }
};
