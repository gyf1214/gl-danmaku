#include "ext.hpp"
#include "vertex/light.hpp"

static const int vertexSize = 18;
static const float radius = 40.0f;
static const float height = 2.0f * M_PI * radius;
static const float uScale = M_PI;
static const float vScale = 1.0f;

static const float lavaflowMaterial[] = { 0.0f, 0.0f, 1.0f, 1.0f };
static const float uvVelocity[] = { 0.0f, 0.01f };

static Vertex vertices[vertexSize];
static int cnt;
static int frame;

proto(Skybox_dynamic, Shader::lambert_dynamic);

protoBuffer(Skybox_dynamic) = {
    { sizeof(vertices), vertices }
};

protoAttrib(Skybox_dynamic) = {
    { "position", Offset(Vertex, position[0]), 3, sizeof(Vertex) },
    { "normal"  , Offset(Vertex, normal[0])  , 3, sizeof(Vertex) },
    { "tangent" , Offset(Vertex, tangent[0]) , 3, sizeof(Vertex) },
    { "uv"      , Offset(Vertex, uv[0])      , 2, sizeof(Vertex) },
};

protoUniform(Skybox_dynamic) = {
    "vMat", "pMat",
    "lightPosition", "lightColor", "ambient", "lightMaterial",
    "material", "diffuse", "normal", "specular", "emission",
    "frame", "uvVelocity"
};

protoTexture(Skybox_dynamic) = {
    Texture::lavaflow
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
    frame = 0;

    pushRectangle(0, M_PI / 6, 0, height);
    pushRectangle(M_PI * 2 / 3, M_PI * 5 / 6, 0, height);
    pushRectangle(M_PI * 4 / 3, M_PI * 3 / 2, 0, height);
}

class Skybox_dynamic : public ProgramBase<Skybox_dynamicProto>, public virtual Object {
    Camera *camera;
    LightManager *lights;
public:
    Skybox_dynamic(Camera *camera, LightManager *lights)
        : camera(camera), lights(lights) {}

    void setup() {
        setupVertices();

        ProgramBase::setup();

        bindBuffer(buffer[0]);

        glUniform1i(uniform[7],  0);
        glUniform1i(uniform[8],  1);
        glUniform1i(uniform[9],  2);
        glUniform1i(uniform[10], 3);

        glUniform1i(uniform[11], 0);
        glUniform2fv(uniform[12], 1, uvVelocity);
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

        glUniform1i(uniform[11], frame++);

        bindTexture(GL_TEXTURE3, texture[0]);
        glUniform4fv(uniform[6], 1, lavaflowMaterial);
        glDrawArrays(GL_TRIANGLES, 0, vertexSize);

        glDisable(GL_CULL_FACE);
    }
};

Object *ObjectBox::skybox_dynamic(Camera *camera, LightManager *light) {
    return create<Skybox_dynamic>(camera, light);
}
