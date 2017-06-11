#include "program_renderer.hpp"
#include "vertex/danmaku.hpp"

proto(Danmaku, Shader::danmaku);

protoBuffer = {
    // { sizeof(uvData), uvData },
};

protoAttrib = {
    { "time"    , Offset(Vertex, time[0])    , 4, sizeof(Vertex) },
    { "position", Offset(Vertex, position[0]), 3, sizeof(Vertex) },
    { "velocity", Offset(Vertex, velocity[0]), 3, sizeof(Vertex) },
    { "uvIndex" , Offset(Vertex, uvIndex[0]) , 4, sizeof(Vertex) },
};

protoUnifom = { "vMat", "pMat", "texture0" };

class Danmaku : public ProgramRenderer<Proto> {
    GLuint texture0;
    Transformer *transform;
public:
    Danmaku(Scene *scene, Transformer *trans)
        : ProgramRenderer(scene), transform(trans) {}

    void setup() {
        ProgramRenderer::setup();

        glUniform1i(uniform[2], 0);
        texture0 = Texture::etama();
    }

    void render() {
        bindProgram();

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);

        glUniformMatrix4fv(uniform[0], 1, GL_FALSE, &scene -> vMat()[0][0]);
        glUniformMatrix4fv(uniform[1], 1, GL_FALSE, &scene -> pMat()[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);

        bindBuffer(transform -> outputBuffer());

        glDrawArrays(GL_POINTS, 0, vertexSize);

        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
    }
};


Renderer *ObjectBox::danmaku(Scene *scene, Transformer *trans) {
    return create<Danmaku>(scene, trans);
}
