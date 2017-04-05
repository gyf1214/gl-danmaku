#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "vertex/light.hpp"
#include "program_renderer.hpp"
#include "scene.hpp"
#include "object_box.hpp"
#include "shader.hpp"
#include "texture.hpp"

static const int circleSize = 128;
static const int wallSize = 6 * circleSize;
static const int floorSize = 4;
static const int vertexSize = wallSize + floorSize;
static const float radius = 20.0f;
static const float heightScale = 0.5f;
static const float height = radius * M_PI * heightScale;
static const float uvScale = 10.0f;

static const float ambient[] = { 0.0f, 0.0f, 0.0f };
static const float lightColor[] = { 800.0f, 480.0f, 320.0f };
static const float lightPosition[] = { 0.0f, 0.0f, - height, 1.0f };
static const float lightColor2[] = { 3.0f, 0.9f, 0.3f };
static const float lightPosition2[] = { 0.0f, 0.0f, -1.0f, 0.0f };
static const float material[] = { 1.0f, 0.0f };

static Vertex vertices[vertexSize];
static int cnt;

proto(Skybox, Shader::lambert);

protoBuffer = {
    { sizeof(vertices), vertices }
};

protoAttrib = {
    { "position", Offset(Vertex, position[0]), 3, sizeof(Vertex) },
    { "normal"  , Offset(Vertex, normal[0])  , 3, sizeof(Vertex) },
    { "uv"      , Offset(Vertex, uv[0])      , 2, sizeof(Vertex) },
};

protoUnifom = {
    "texture0", "vMat", "pMat",
    "lightPosition", "lightColor", "ambient",
    "material",
};

static void push(int x, int y, float norm) {
    float t = (float)x / (float)circleSize;
    float angle = t * M_PI * 2.0f;
    float yy = y * height;

    vertices[cnt].uv = glm::vec2(t * uvScale, y * uvScale / 4.0f * heightScale);
    vertices[cnt].position = glm::vec3(radius * cos(angle), radius * sin(angle), yy);
    vertices[cnt].normal = glm::vec3(-cos(norm), -sin(norm), 0.0f);
    ++cnt;
}

static void setupVertices() {
    cnt = 0;
    const float angle2 = 2.0f * M_PI / (float)circleSize;

    for (int i = 0; i < circleSize; ++i) {
        float angle0 = (float)i * angle2;
        float angle1 = (float)(i + 1) * angle2;
        push(i    ,  1, angle0);
        push(i    , -1, angle0);
        push(i + 1,  1, angle1);
        push(i + 1,  1, angle1);
        push(i    , -1, angle0);
        push(i + 1, -1, angle1);
    }

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            float xx = ((float)i * 2.0f - 1.0f) * radius;
            float yy = ((float)j * 2.0f - 1.0f) * radius;
            vertices[cnt].position = glm::vec3(xx, yy, -height);
            vertices[cnt].normal = glm::vec3(0.0f, 0.0f, 1.0f);
            vertices[cnt++].uv = glm::vec2((float)i, (float)j);
        }
    }
}

class Skybox : public ProgramRenderer<Proto> {
    GLuint wallTex, floorTex;
public:
    Skybox(Scene *scene) : ProgramRenderer(scene) {}

    void setup() {
        setupVertices();

        ProgramRenderer::setup();

        bindBuffer(buffer[0]);

        glUniform4fv(uniform[3], 1, lightPosition);
        glUniform3fv(uniform[4], 1, lightColor);
        glUniform3fv(uniform[5], 1, ambient);
        glUniform2fv(uniform[6], 1, material);

        glUniform1i(uniform[0], 0);
        wallTex = Texture::sceneWall();
        floorTex = Texture::sceneFloor();
    }

    void render() {
        bindProgram();

        glEnable(GL_DEPTH_TEST);

        glUniformMatrix4fv(uniform[1], 1, GL_FALSE, &scene -> vMat()[0][0]);
        glUniformMatrix4fv(uniform[2], 1, GL_FALSE, &scene -> pMat()[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTex);
        glUniform4fv(uniform[3], 1, lightPosition2);
        glUniform3fv(uniform[4], 1, lightColor2);

        glDrawArrays(GL_TRIANGLE_STRIP, wallSize, 4);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, wallTex);
        glUniform4fv(uniform[3], 1, lightPosition);
        glUniform3fv(uniform[4], 1, lightColor);

        glDrawArrays(GL_TRIANGLES, 0, wallSize);

        glDisable(GL_DEPTH_TEST);
    }
};

Renderer *ObjectBox::skybox(Scene *scene) {
    return create<Skybox>(scene);
}
