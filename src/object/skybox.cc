#include "program_renderer.hpp"
#include "vertex/light.hpp"

static const int circleSize = 128;
static const int wallSize1 = 6 * circleSize;
static const int wallSize2 = 6 * circleSize;
static const int wallSize = wallSize1 + wallSize2;
static const int floorSize = 4;
static const int vertexSize = wallSize + floorSize;
static const float radius = 40.0f;
static const float height1 = 1.0f * M_PI * radius;
static const float height2 = 1.0f * M_PI * radius;
static const float height = height1 + height2;
static const float uScale = 16.0f;
static const float vScale = 64.0f;

static const float wallMaterial[]  = { 1.0f, 0.5f, 0.8f, 100.0f };
static const float floorMaterial[] = { 0.0f, 0.0f, 1.0f, 1.0f };

static Vertex vertices[vertexSize];
static int cnt;

proto(Skybox, Shader::lambert);

protoBuffer = {
    { sizeof(vertices), vertices }
};

protoAttrib = {
    { "position", Offset(Vertex, position[0]), 3, sizeof(Vertex) },
    { "normal"  , Offset(Vertex, normal[0])  , 3, sizeof(Vertex) },
    { "tangent" , Offset(Vertex, tangent[0]) , 3, sizeof(Vertex) },
    { "uv"      , Offset(Vertex, uv[0])      , 2, sizeof(Vertex) },
};

protoUnifom = {
    "vMat", "pMat",
    "lightPosition", "lightColor", "ambient", "lightMaterial",
    "material", "diffuse", "normal", "specular", "emission"
};

static void pushVertex(float theta, float z) {
    float u = theta / (2.0f * M_PI);
    float v = z / height;

    vertices[cnt].uv = vec2(u * uScale, v * vScale);
    vertices[cnt].position = vec3(radius * cos(theta), radius * sin(theta), z);
    vertices[cnt].normal = vec3(-cos(theta), -sin(theta), 0.0f);
    vertices[cnt].tangent = vec3(sin(theta), -cos(theta), 0.0f);
    ++cnt;
}

static void pushRectangle(float theta1, float theta2, float z1, float z2) {
    pushVertex(theta1, z1);
    pushVertex(theta1, z2);
    pushVertex(theta2, z2);
    pushVertex(theta2, z2);
    pushVertex(theta2, z1);
    pushVertex(theta1, z1);
}

static void setupVertices() {
    cnt = 0;

    for (int i = 0; i < circleSize; ++i) {
        float theta1 = (float)i / (float)circleSize * 2.0f * M_PI;
        float theta2 = (float)(i + 1) / (float)circleSize * 2.0f * M_PI;

        pushRectangle(theta1, theta2, 0, height1);
    }

    for (int i = 0; i < circleSize; ++i) {
        float theta1 = (float)i / (float)circleSize * 2.0f * M_PI;
        float theta2 = (float)(i + 1) / (float)circleSize * 2.0f * M_PI;

        pushRectangle(theta1, theta2, height1, height1 + height2);
    }

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            float xx = ((float)j * 2.0f - 1.0f) * radius;
            float yy = ((float)i * 2.0f - 1.0f) * radius;
            vertices[cnt].position = vec3(xx, yy, 0.0f);
            vertices[cnt].normal = vec3(0.0f, 0.0f, 1.0f);
            vertices[cnt].tangent = vec3(0.0f, 0.0f, 0.0f);
            vertices[cnt++].uv = vec2((float)i, (float)j);
        }
    }
}

class Skybox : public ProgramRenderer<Proto> {
    GLuint lava, wall, wallLava, normal, white, wall1, wall2;
public:
    Skybox(Scene *scene) : ProgramRenderer(scene) {}

    void setup() {
        setupVertices();

        ProgramRenderer::setup();

        bindBuffer(buffer[0]);

        glUniform1i(uniform[7],  0);
        glUniform1i(uniform[8],  1);
        glUniform1i(uniform[9],  2);
        glUniform1i(uniform[10], 3);
        wall = Texture::wall();
        lava = Texture::lava();
        wallLava = Texture::wallLava();
        normal = Texture::wallNormal();
        white = Texture::white();
        wall1 = Texture::wall1();
        wall2 = Texture::wall2();
    }

    void render() {
        bindProgram();

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glUniformMatrix4fv(uniform[0], 1, GL_FALSE, &scene -> vMat()[0][0]);
        glUniformMatrix4fv(uniform[1], 1, GL_FALSE, &scene -> pMat()[0][0]);

        Light light = scene -> light();

        glUniform4fv(uniform[2], 1, &light.position[0]);
        glUniform3fv(uniform[3], 1, &light.color[0]);
        glUniform3fv(uniform[4], 1, &light.ambient[0]);
        glUniform4fv(uniform[5], 1, &light.material[0]);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, lava);

        glUniform4fv(uniform[6], 1, floorMaterial);
        glDrawArrays(GL_TRIANGLE_STRIP, wallSize, 4);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, wall);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, white);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, wall1);

        glUniform4fv(uniform[6], 1, wallMaterial);
        glDrawArrays(GL_TRIANGLES, 0, wallSize1);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, wall2);

        glUniform4fv(uniform[6], 1, wallMaterial);
        glDrawArrays(GL_TRIANGLES, wallSize1, wallSize2);

        glDisable(GL_CULL_FACE);
    }
};

Renderer *ObjectBox::skybox(Scene *scene) {
    return create<Skybox>(scene);
}
