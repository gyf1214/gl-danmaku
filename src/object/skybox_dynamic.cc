#include "ext.hpp"
#include "vertex/base.hpp"

static const int vertexSize = 18;
static const float radius = 35.0f;
static const float height = 1.4f * M_PI * 40.0f;
static const float uScale = M_PI;
static const float vScale = 1.0f;

static const float uvVelocity[] = { 0.0f, 0.1f };

static Vertex vertices[vertexSize];
static int cnt;

proto(Skybox_dynamic, Shader::dynamic);

protoBuffer(Skybox_dynamic) = {
    { sizeof(vertices), vertices }
};

protoAttrib(Skybox_dynamic) = {
    { "position", Offset(Vertex, position[0]), 3, sizeof(Vertex) },
    { "uv"      , Offset(Vertex, uv[0])      , 2, sizeof(Vertex) },
};

protoUniform(Skybox_dynamic) = {
    "vMat", "pMat", "frame", "uvVelocity",
    "texture0"
};

protoTexture(Skybox_dynamic) = { Texture::lavaflow };

static void pushVertex(float theta, float z) {
    float u = theta / (2.0f * M_PI);
    float v = z / height;
    vertices[cnt].uv = vec2(u * uScale, v * vScale);
    vertices[cnt].position = vec3(radius * cos(theta), radius * sin(theta), z);
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
    pushRectangle(0, M_PI / 6, 0, height);
    pushRectangle(M_PI * 2 / 3, M_PI * 5 / 6, 0, height);
    pushRectangle(M_PI * 4 / 3, M_PI * 3 / 2, 0, height);
}

class Skybox_dynamic : public ProgramBase<Skybox_dynamicProto>, public virtual Object {
    Camera *camera;
    LightManager *lights;
    float frame;
public:
    Skybox_dynamic(Camera *camera, LightManager *lights)
        : camera(camera), lights(lights) {}

    void setup() {
        setupVertices();

        ProgramBase::setup();

        bindBuffer(buffer[0]);

        glUniform2fv(uniform[3], 1, uvVelocity);
        glUniform1i(uniform[4], 0);

        frame = 0.0f;
    }

    void render() {
        if (lights->pass() > 0) return;

        bindProgram();

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glUniformMatrix4fv(uniform[0], 1, GL_FALSE, &camera->vMat()[0][0]);
        glUniformMatrix4fv(uniform[1], 1, GL_FALSE, &camera->pMat()[0][0]);

        frame += Application::elapse();
        glUniform1f(uniform[2], frame);
        bindTexture(GL_TEXTURE0, texture[0]);

        glDrawArrays(GL_TRIANGLES, 0, vertexSize);

        glDisable(GL_CULL_FACE);
    }
};

Object *ObjectBox::skybox_dynamic(Camera *camera, LightManager *light) {
    return create<Skybox_dynamic>(camera, light);
}
