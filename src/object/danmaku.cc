#include <GL/glew.h>
#include <glm/glm.hpp>
#include "vertex.hpp"
#include "transform_renderer.hpp"
#include "program_renderer.hpp"
#include "scene.hpp"
#include "object_box.hpp"
#include "shader.hpp"
#include "texture.hpp"

static const GLfloat pointSize[] = { 0.5f, 0.5f };

static const GLfloat uvData[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
};

proto(Danmaku, Shader::danmaku);

protoBuffer = {
    { sizeof(uvData), uvData },
};

protoAttrib = {
    { "time"    , Offset(Vertex, time[0])    , 4, sizeof(Vertex) },
    { "position", Offset(Vertex, position[0]), 3, sizeof(Vertex) },
    { "uv"      , (void *)0                  , 2, 0 },
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

        bindAttribute(transform -> outputBuffer(), 0);
        glVertexAttribDivisor(attribute[0], 1);
        bindAttribute(transform -> outputBuffer(), 1);
        glVertexAttribDivisor(attribute[1], 1);
        bindAttribute(buffer[0], 2);


        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, vertexSize);
    }
};


Renderer *ObjectBox::danmaku(Scene *scene, Transformer *trans) {
    return create<Danmaku>(scene, trans);
}
