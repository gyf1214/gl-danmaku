#ifndef __PROGRAM_RENDERER
#define __PROGRAM_RENDERER

#include <GL/glew.h>
#include "renderer.hpp"
#include "util.hpp"

struct BufferProto {
    GLsizeiptr size;
    const GLvoid *data;
	GLenum usage;

    BufferProto(GLsizeiptr _size, const GLvoid *_data = NULL,
                GLenum _usage = GL_DYNAMIC_DRAW)
        : size(_size), data(_data), usage(_usage) {}
};

struct AttribProto {
    const GLchar *name;
    const GLvoid *offset;
    GLint size;
    GLsizei stride;
    GLenum type;
    GLboolean normalized;

    AttribProto(const GLchar *_name, const GLvoid *_offset,
                GLint _size, GLsizei _stride, GLenum _type = GL_FLOAT,
                GLboolean _normalized = GL_FALSE)
        : name(_name), offset(_offset), size(_size), stride(_stride),
          type(_type), normalized(_normalized) {}
};

typedef const char *UniformProto;

#define protoOpen(name, program) typedef struct name##Proto {\
    static constexpr const char *Name = #name;\
    static constexpr auto Program = program;\
    static const BufferProto Buffers[];\
    static const AttribProto Attributes[];\
    static const UniformProto Uniforms[];

#define proto(name, program) protoOpen(name, program)\
} Proto

#define protoBuffer const BufferProto Proto::Buffers[]
#define protoAttrib const AttribProto Proto::Attributes[]
#define protoUnifom const UniformProto Proto::Uniforms[]

template <typename Proto>
class ProgramBase {
protected:
    static constexpr const char *Name = Proto::Name;
    static constexpr int AttribSize = sizeof(Proto::Attributes) / sizeof(Proto::Attributes[0]);
    static constexpr int UniformSize = sizeof(Proto::Uniforms) / sizeof(Proto::Uniforms[0]);
    static constexpr int BufferSize = sizeof(Proto::Buffers) / sizeof(Proto::Buffers[0]);

    GLuint program;
    GLuint vertexArray;
    GLuint buffer[BufferSize ? BufferSize : 1];
    GLuint attribute[AttribSize ? AttribSize : 1];
    GLuint uniform[UniformSize ? UniformSize : 1];

    virtual void setupProgram() {
        LOG << "setup program";
        program = Proto::Program();
        LOG << "program: " << program;
    }

    virtual void setupArray() {
        glGenVertexArrays(1, &vertexArray);
        LOG << "vertex array: " << vertexArray;
    }

    virtual void setupBuffers() {
        if (!BufferSize) return;
        glGenBuffers(BufferSize, buffer);
        LOG << "generate " << BufferSize << " buffers";
        for (int i = 0; i < BufferSize; ++i) {
            LOG << "set buffer " << i << " : " << buffer[i];
            const auto &proto = Proto::Buffers[i];

            glBindBuffer(GL_ARRAY_BUFFER, buffer[i]);
            glBufferData(GL_ARRAY_BUFFER, proto.size, proto.data, proto.usage);
        }
    }

    virtual void setupAttributes() {
        LOG << "setup " << AttribSize << " attributes";
        for (int i = 0; i < AttribSize; ++i) {
            const auto &proto = Proto::Attributes[i];
            attribute[i] = glGetAttribLocation(program, proto.name);
            LOG << proto.name << ": " << attribute[i];

            glEnableVertexAttribArray(attribute[i]);
        }
    }

    virtual void setupUniforms() {
        LOG << "setup " << UniformSize << " uniforms";
        for (int i = 0; i < UniformSize; ++i) {
            uniform[i] = glGetUniformLocation(program, Proto::Uniforms[i]);
            LOG << Proto::Uniforms[i] << ": " << uniform[i];
        }
    }

    virtual void bindAttribute(GLuint buf, int index) {
        glBindBuffer(GL_ARRAY_BUFFER, buf);
        const auto &proto = Proto::Attributes[index];
        glVertexAttribPointer(attribute[index], proto.size, proto.type,
                              proto.normalized, proto.stride, proto.offset);
    }

    virtual void bindBuffer(GLuint buf) {
        glBindBuffer(GL_ARRAY_BUFFER, buf);
        for (int i = 0; i < AttribSize; ++i) {
            bindAttribute(buf, i);
        }
    }

    virtual void bindProgram() {
        glUseProgram(program);
        glBindVertexArray(vertexArray);
    }

    virtual void setupAll() {
        LOG << "setup object: " << ProgramBase<Proto>::Name;
        setupProgram();
        glUseProgram(program);
        setupArray();
        glBindVertexArray(vertexArray);
        setupBuffers();
        setupAttributes();
        setupUniforms();
    }

    virtual void resetAll() {
        LOG << "reset object: " << ProgramBase<Proto>::Name;
        LOG << "delete array: " << vertexArray;
        if (BufferSize) {
            LOG << "delete " << BufferSize << " buffers";
            glDeleteBuffers(BufferSize, buffer);
        }
    }
};

template <typename Proto>
class ProgramRenderer : public ProgramBase<Proto>, public Renderer {
protected:
    ProgramRenderer(Scene *scene) : Renderer(scene) {}
public:
    void setup() {
        this -> setupAll();
    }

    void reset() {
        this -> resetAll();
    }
};

#endif
