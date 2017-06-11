#include "program_renderer.hpp"
#include "model.hpp"

using namespace glm;
using namespace std;

typedef mmd::pmx::Vertex Vertex;

proto(Character, Shader::character);

protoBuffer = {
    { 0, NULL },
    { 0, NULL }
};

protoAttrib = {
    { "position", Offset(Vertex, position[0]), 3, sizeof(Vertex) },
    { "normal"  , Offset(Vertex, normal[0])  , 3, sizeof(Vertex) },
    { "uv"      , Offset(Vertex, uv[0])      , 2, sizeof(Vertex) },
};

protoUnifom = {
    "mMat", "vMat", "pMat",
    "lightPosition", "lightColor", "ambient", "lightMaterial",
    "diffuse", "specular", "texture0"
};

class Character : public ProgramRenderer<Proto> {
    Model *model;
public:
    Character(Scene *scene, Model *model)
        : ProgramRenderer(scene), model(model) {}

    void setupBuffers() {
        int v = model->mesh.vertex.size();
        int s = model->mesh.surface.size();

        glGenBuffers(BufferSize, buffer);
        LOG << "generate " << BufferSize << " buffers";

        glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
        glBufferData(GL_ARRAY_BUFFER, v * sizeof(Vertex),
            &model->mesh.vertex[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, s * sizeof(mmd::pmx::Surface),
            &model->mesh.surface[0], GL_STATIC_DRAW);
    }

    void setup() {
        ProgramRenderer::setup();

        bindBuffer(buffer[0]);

        glUniform1i(uniform[9], 0);
        model->loadTextures();
    }

    void render() {
        bindProgram();

        mat4 mMat(1.0f);
        swap(mMat[1], mMat[2]);
        mMat = scale(vec3(0.1f, 0.1f, 0.1f)) * mMat;
        mMat = translate(vec3(0.0f, 4.0f, 40.0f)) * mMat;

        glUniformMatrix4fv(uniform[0], 1, GL_FALSE, &mMat[0][0]);
        glUniformMatrix4fv(uniform[1], 1, GL_FALSE, &scene->vMat()[0][0]);
        glUniformMatrix4fv(uniform[2], 1, GL_FALSE, &scene -> pMat()[0][0]);

        Light light = scene->light();
        bool ambient = !scene->pass();

        glUniform3fv(uniform[5], 1, &light.ambient[0]);
        glUniform4fv(uniform[3], 1, &light.position[0]);
        glUniform3fv(uniform[4], 1, &light.color[0]);
        glUniform4fv(uniform[6], 1, &light.material[0]);

        glActiveTexture(GL_TEXTURE0);

        int sum = 0;

        for (int i = 0; i < model->materials.size(); ++i) {
            const auto &material = model->materials[i];

            if (ambient) {
                glUniform3fv(uniform[5], 1, &material.ambient[0]);
            }
            glUniform4fv(uniform[7], 1, &material.diffuse[0]);
            glUniform4fv(uniform[8], 1, &material.specular[0]);

            glBindTexture(GL_TEXTURE_2D, model->texture(material.texture));

            glDrawElements(GL_TRIANGLES, material.count,
                GL_UNSIGNED_INT, (void *)(sum * sizeof(GLuint)));

            sum += material.count;
        }
    }
};

Renderer *ObjectBox::character(Scene *scene, Model *model) {
    return create<Character>(scene, model);
}
