#include "character.hpp"

using namespace glm;
using namespace std;
using namespace mmd;

using mmd::pmx::Vertex;
using mmd::physics::Motion;

Character::Character(Scene *scene, Model *model, mmd::vmd::Motion *motion)
        : ProgramRenderer(scene), motion(Motion::create()),
          model(model), vMotion(motion) {}

Character::~Character() {
    delete motion;
}

void Character::setupBuffers() {
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

    glBindBuffer(GL_UNIFORM_BUFFER, buffer[2]);
    glBufferData(GL_UNIFORM_BUFFER, bones.size() * sizeof(mat4),
                 NULL, GL_DYNAMIC_DRAW);
}

void Character::setup() {
    motion->loadModel(model);
    motion->loadMotion(vMotion);
    bones.resize(model->bones.size());
    morphs.resize(model->morphs.size());

    ProgramRenderer::setup();

    bindBuffer(buffer[0]);

    GLuint uboIndex = glGetUniformBlockIndex(program, "Bones");
    glUniformBlockBinding(program, uboIndex, uboBinding);

    glUniform1i(uniform[9], 0);
    glUniform1i(uniform[11], 1);
    glUniform1i(uniform[12], morphs.size());

    model->loadTextures();
    model->loadMorphTexture();
}

void Character::render() {
    bindProgram();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    mat4 mMat(1.0f);
    swap(mMat[1], mMat[2]);
    mMat = scale(vec3(0.1f, 0.1f, 0.1f)) * mMat;
    mMat = translate(vec3(0.0f, 4.0f, 10.0f)) * mMat;

    glUniformMatrix4fv(uniform[0], 1, GL_FALSE, &mMat[0][0]);
    glUniformMatrix4fv(uniform[1], 1, GL_FALSE, &scene->vMat()[0][0]);
    glUniformMatrix4fv(uniform[2], 1, GL_FALSE, &scene->pMat()[0][0]);

    Light light = scene->light();

    glUniform4fv(uniform[3], 1, &light.position[0]);
    glUniform3fv(uniform[4], 1, &light.color[0]);
    glUniform4fv(uniform[6], 1, &light.material[0]);
    glUniform1fv(uniform[13], morphs.size(), &morphs[0]);

    glBindBuffer(GL_UNIFORM_BUFFER, buffer[2]);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, bones.size() * sizeof(mat4),
                    &bones[0][0][0]);
    glBindBufferBase(GL_UNIFORM_BUFFER, uboBinding, buffer[2]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_BUFFER, model->morphTexture());

    glActiveTexture(GL_TEXTURE0);

    int sum = 0;

    for (int i = 0; i < model->materials.size(); ++i) {
        const auto &material = model->materials[i];

        glUniform3fv(uniform[5], 1, &material.ambient[0]);
        glUniform4fv(uniform[7], 1, &material.diffuse[0]);
        glUniform4fv(uniform[8], 1, &material.specular[0]);

        glBindTexture(GL_TEXTURE_2D, model->texture(material.texture));

        glUniform1f(uniform[10], 1.0f);
        glCullFace(GL_FRONT);
        glDrawElements(GL_TRIANGLES, material.count,
            GL_UNSIGNED_INT, (void *)(sum * sizeof(GLuint)));

        if (material.flag & mmd::pmx::Material::DoubleSided) {
            glUniform1f(uniform[10], -1.0f);
            glCullFace(GL_BACK);
            glDrawElements(GL_TRIANGLES, material.count,
                GL_UNSIGNED_INT, (void *)(sum * sizeof(GLuint)));
        }

        sum += material.count;
    }

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

void Character::update() {
    static int frame = 0;
    motion->updateKey(frame / 2);
    motion->updatePhysics(Application::elapse);
    ++frame;

    for (int i = 0; i < bones.size(); ++i) {
        bones[i] = motion->skin(i);
    }
    for (int i = 0; i < morphs.size(); ++i) {
        morphs[i] = motion->face(i);
    }
}

void Character::reset() {
    ProgramRenderer::reset();

    motion->reset();
}

Renderer *ObjectBox::character(Scene *scene, Model *model, vmd::Motion *motion) {
    return create<Character>(scene, model, motion);
}
