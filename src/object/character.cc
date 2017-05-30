#include "program_renderer.hpp"
#include "model.hpp"

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
    "vMat", "pMat",
    "lightPosition", "lightColor", "ambient", "lightMaterial",
    "diffuse", "specular", "texture0"
};

class Character : public ProgramRenderer<Proto> {
    GLuint white;
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

        white = Texture::white();
        glUniform1i(uniform[8], 0);
    }

    void render() {
        bindProgram();

        glEnable(GL_DEPTH_TEST);

        glm::mat4 vMat = scene -> vMat();
        // vMat = vMat * glm::translate(glm::vec3(9.0f, 9.0f, 25.0f));
        glUniformMatrix4fv(uniform[0], 1, GL_FALSE, &vMat[0][0]);
        glUniformMatrix4fv(uniform[1], 1, GL_FALSE, &scene -> pMat()[0][0]);

        Light light = scene -> light();

        glUniform4fv(uniform[2], 1, &light.position[0]);
        glUniform3fv(uniform[3], 1, &light.color[0]);
        glUniform3fv(uniform[4], 1, &light.ambient[0]);
        glUniform4fv(uniform[5], 1, &light.material[0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, white);

        glDrawElements(GL_TRIANGLES, model->mesh.surface.size() * 3,
            GL_UNSIGNED_INT, (void *)0);

        glDisable(GL_DEPTH_TEST);
    }
};

Renderer *ObjectBox::character(Scene *scene, Model *model) {
    return create<Character>(scene, model);
}
