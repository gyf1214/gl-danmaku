#include "ext.hpp"
#include "vertex/light.hpp"

static const int circleSize = 128;
static const int wallSize = 6 * circleSize;
static const int floorSize = 4;
static const int vertexSize = wallSize + floorSize;
static const float radius = 40.0f;
static const float heightScale = 1.0f;
static const float height = radius * M_PI * 2.0f * heightScale;
static const float uvScale = 20.0f;

static const float wallMaterial[]  = { 1.0f, 0.5f, 0.8f, 100.0f };
static const float floorMaterial[] = { 0.0f, 0.0f, 1.0f, 1.0f };

static Vertex vertices[vertexSize];
static int cnt;

proto(Skybox, Shader::lambert);

protoBuffer(Skybox) = {
    { sizeof(vertices), vertices }
};

protoAttrib(Skybox) = {
    { "position", Offset(Vertex, position[0]), 3, sizeof(Vertex) },
    { "normal"  , Offset(Vertex, normal[0])  , 3, sizeof(Vertex) },
    { "tangent" , Offset(Vertex, tangent[0]) , 3, sizeof(Vertex) },
    { "uv"      , Offset(Vertex, uv[0])      , 2, sizeof(Vertex) },
};

protoUniform(Skybox) = {
    "vMat", "pMat",
    "lightPosition", "lightColor", "ambient", "lightMaterial",
    "material", "diffuse", "normal", "specular", "emission"
};

protoTexture(Skybox) = {
    Texture::lava, Texture::wall, Texture::wallNormal,
    Texture::white, Texture::wallLava
};

static void push(int x, int y, float norm) {
    float t = (float)x / (float)circleSize;
    float angle = t * M_PI * 2.0f;
    float yy = y * height;

    vertices[cnt].uv = vec2(t * uvScale, y * uvScale * heightScale);
    vertices[cnt].position = vec3(radius * cos(angle), -radius * sin(angle), yy);
    vertices[cnt].normal = vec3(-cos(norm), sin(norm), 0.0f);
    vertices[cnt].tangent = vec3(-sin(norm), -cos(norm), 0.0f);
    // vertices[cnt].tangent = vec3(0.0f, 0.0f, 0.0f);
    ++cnt;
}

static void setupVertices() {
    cnt = 0;
    const float angle2 = 2.0f * M_PI / (float)circleSize;

    for (int i = 0; i < circleSize; ++i) {
        float angle0 = (float)i * angle2;
        float angle1 = (float)(i + 1) * angle2;
        push(i    ,  1, angle0);
        push(i    ,  0, angle0);
        push(i + 1,  1, angle1);
        push(i + 1,  1, angle1);
        push(i    ,  0, angle0);
        push(i + 1,  0, angle1);
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

class Skybox : public ProgramBase<SkyboxProto>, public virtual Object {
    Camera *camera;
    LightManager *lights;
public:
    Skybox(Camera *camera, LightManager *lights)
        : camera(camera), lights(lights) {}

    void setup() {
        setupVertices();

        ProgramBase::setup();

        bindBuffer(buffer[0]);

        glUniform1i(uniform[7], 1);
        glUniform1i(uniform[8], 2);
        glUniform1i(uniform[9], 3);
    }

    void render() {
        bindProgram();

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glUniformMatrix4fv(uniform[0], 1, GL_FALSE, &camera->vMat()[0][0]);
        glUniformMatrix4fv(uniform[1], 1, GL_FALSE, &camera->pMat()[0][0]);

        Light light = lights->current();

        glUniform4fv(uniform[2], 1, &light.position[0]);
        glUniform3fv(uniform[3], 1, &light.color[0]);
        glUniform3fv(uniform[4], 1, &light.ambient[0]);
        glUniform4fv(uniform[5], 1, &light.material[0]);

        bindTextures();

        glUniform1i(uniform[10], 0);
        glUniform4fv(uniform[6], 1, floorMaterial);
        glDrawArrays(GL_TRIANGLE_STRIP, wallSize, 4);

        glUniform1i(uniform[10], 4);
        glUniform4fv(uniform[6], 1, wallMaterial);
        glDrawArrays(GL_TRIANGLES, 0, wallSize);

        glDisable(GL_CULL_FACE);
    }
};
