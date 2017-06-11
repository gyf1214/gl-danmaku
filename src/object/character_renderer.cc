#include "program_renderer.hpp"
#include "mmd-physics/motion.hpp"

using namespace glm;
using namespace std;
using namespace mmd;

using mmd::pmx::Vertex;
using mmd::physics::Motion;

proto(Character, Shader::character);

protoBuffer = {
    { 0, NULL },
    { 0, NULL },
    { 0, NULL },
};

protoAttrib = {
    { "position"  , Offset(Vertex, position[0])   , 3, sizeof(Vertex) },
    { "normal"    , Offset(Vertex, normal[0])     , 3, sizeof(Vertex) },
    { "uv"        , Offset(Vertex, uv[0])         , 2, sizeof(Vertex) },
    { "boneIndex" , Offset(Vertex, BDEF.bone[0])  , 4, sizeof(Vertex), true, GL_INT },
    { "boneWeight", Offset(Vertex, BDEF.weight[0]), 4, sizeof(Vertex) }
};

protoUnifom = {
    "mMat", "vMat", "pMat",
    "lightPosition", "lightColor", "ambient", "lightMaterial",
    "diffuse", "specular", "texture0", "side",
    "morphData", "morphCount", "morphs"
};

class CharacterRenderer : public ProgramBase<Proto>, public Character {
protected:
    static const int uboBinding = 1;

    mmd::physics::Motion *motion;
    Model *model;
    mmd::vmd::Motion *vMotion;

    std::vector<glm::mat4> bones;
    std::vector<float> morphs;
    static const glm::mat4 preTransform;
    static const glm::mat4 invTransform;

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

        glBindBuffer(GL_UNIFORM_BUFFER, buffer[2]);
        glBufferData(GL_UNIFORM_BUFFER, bones.size() * sizeof(mat4),
                     NULL, GL_DYNAMIC_DRAW);
    }
public:
    CharacterRenderer(Scene *scene, Model *model, mmd::vmd::Motion *vMotion)
        : Character(scene), motion(Motion::create()),
          model(model), vMotion(vMotion) {

        transform.mat = mat4(1.0f);
        transform.reset = false;
        frame.play = false;
        frame.current = 0.0f;
    }

    ~CharacterRenderer() {
        delete motion;
    }

    void setup() {
        motion->loadModel(model);
        motion->loadMotion(vMotion);

        motion->updateGlobal(invTransform * transform.mat * preTransform);
        motion->updateKey(frame.current);

        motion->loadBody();
        transform.reset = false;

        bones.resize(model->bones.size());
        morphs.resize(model->morphs.size());

        setupAll();

        bindBuffer(buffer[0]);

        GLuint uboIndex = glGetUniformBlockIndex(program, "Bones");
        glUniformBlockBinding(program, uboIndex, uboBinding);

        glUniform1i(uniform[9], 0);
        glUniform1i(uniform[11], 1);
        glUniform1i(uniform[12], morphs.size());

        model->loadTextures();
        model->loadMorphTexture();
    }

    void render() {
        bindProgram();

        glEnable(GL_CULL_FACE);

        mat4 mMat = transform.mat * preTransform;

        glUniformMatrix4fv(uniform[0], 1, GL_FALSE, &mMat[0][0]);
        glUniformMatrix4fv(uniform[1], 1, GL_FALSE, &scene->vMat()[0][0]);
        glUniformMatrix4fv(uniform[2], 1, GL_FALSE, &scene->pMat()[0][0]);

        Light light = scene->light();
        bool ambient = !scene->pass();

        glUniform3fv(uniform[5], 1, &light.ambient[0]);
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

            if (ambient) glUniform3fv(uniform[5], 1, &material.ambient[0]);
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

        glDisable(GL_CULL_FACE);
    }

    void update() {
        updateMotion();
        updateFrame();

        motion->updateKey(frame.current);
        motion->updateGlobal(invTransform * transform.mat * preTransform);
        if (transform.reset) {
            motion->resetPhysics();
            transform.reset = false;
        }
        motion->updatePhysics(Application::elapse);

        for (int i = 0; i < bones.size(); ++i) {
            bones[i] = motion->skin(i);
        }
        for (int i = 0; i < morphs.size(); ++i) {
            morphs[i] = motion->face(i);
        }
    }

    void reset() {
        resetAll();

        motion->reset();
    }
};

const mat4 CharacterRenderer::preTransform(
    0.1f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.1f, 0.0f,
    0.0f, 0.1f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
);

const mat4 CharacterRenderer::invTransform = inverse(preTransform);

Character *ObjectBox::character(Scene *scene, Model *model, vmd::Motion *motion) {
    return create<CharacterRenderer>(scene, model, motion);
}
