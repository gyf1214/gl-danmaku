#include <GL/glew.h>
#include <glm/glm.hpp>
#include "vertex.hpp"
#include "transform_renderer.hpp"
#include "program_renderer.hpp"
#include "scene.hpp"
#include "object_box.hpp"
#include "shader.hpp"
#include "texture.hpp"

static const GLfloat pointSize[] = { 0.5f, 0.2f };

proto(Danmaku, Shader::danmaku);

protoBuffer = {
    // { sizeof(uvData), uvData },
};

protoAttrib = {
    { "time"    , Offset(Vertex, time[0])    , 4, sizeof(Vertex) },
    { "position", Offset(Vertex, position[0]), 3, sizeof(Vertex) },
    { "velocity", Offset(Vertex, velocity[0]), 3, sizeof(Vertex) },
};

protoUnifom = { "size", "vMat", "pMat", "texture0" };

class Danmaku : public ProgramRenderer<Proto> {
    GLuint texture0;
    Transformer *transform;
public:
    Danmaku(Scene *scene, Transformer *trans)
        : ProgramRenderer(scene), transform(trans) {}

    void setup() {
        ProgramRenderer::setup();

        glUniform1i(uniform[3], 0);
        texture0 = Texture::small();

        glUniform2fv(uniform[0], 1, pointSize);
    }

    void render() {
        bindProgram();

        glUniformMatrix4fv(uniform[1], 1, GL_FALSE, scene -> vMat());
        glUniformMatrix4fv(uniform[2], 1, GL_FALSE, scene -> pMat());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);

        bindBuffer(transform -> outputBuffer());

        glDrawArrays(GL_POINTS, 0, vertexSize);
    }
};


Renderer *ObjectBox::danmaku(Scene *scene, Transformer *trans) {
    return create<Danmaku>(scene, trans);
}
