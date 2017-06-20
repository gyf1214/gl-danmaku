#ifndef __CORE_EXT_PROGRAM
#define __CORE_EXT_PROGRAM

#include "../ext.hpp"

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

    bool integer;

    AttribProto(const GLchar *_name, const GLvoid *_offset,
                GLint _size, GLsizei _stride, bool integer = false,
                GLenum _type = GL_FLOAT, GLboolean _normalized = GL_FALSE)
        : name(_name), offset(_offset), size(_size), stride(_stride),
          type(_type), normalized(_normalized), integer(integer) {}
};

typedef const char *UniformProto;
typedef GLuint (*TextureProto)(void);

#define protoOpen(name, program) struct name##Proto {\
    static constexpr const char *Name = #name;\
    static constexpr auto Program = program;\
    static const BufferProto Buffers[];\
    static const AttribProto Attributes[];\
    static const UniformProto Uniforms[];\
    static const TextureProto Textures[];

#define proto(name, program) protoOpen(name, program)\
}

#define protoBuffer(name) const BufferProto name##Proto::Buffers[]
#define protoAttrib(name) const AttribProto name##Proto::Attributes[]
#define protoUniform(name) const UniformProto name##Proto::Uniforms[]
#define protoTexture(name) const TextureProto name##Proto::Textures[]

template <typename Proto>
class ProgramBase : public virtual BaseObject {
protected:
    static constexpr const char *Name = Proto::Name;
    static constexpr int AttribSize = sizeof(Proto::Attributes) / sizeof(AttribProto);
    static constexpr int UniformSize = sizeof(Proto::Uniforms) / sizeof(UniformProto);
    static constexpr int BufferSize = sizeof(Proto::Buffers) / sizeof(BufferProto);
    static constexpr int TextureSize = sizeof(Proto::Textures) / sizeof(TextureProto);

    GLuint program;
    GLuint vertexArray;
    GLuint buffer[BufferSize ? BufferSize : 1];
    GLuint attribute[AttribSize ? AttribSize : 1];
    GLuint uniform[UniformSize ? UniformSize : 1];
    GLuint texture[TextureSize ? TextureSize : 1];

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
            if (proto.size > 0) {
                glBufferData(GL_ARRAY_BUFFER, proto.size, proto.data, proto.usage);
            }
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

    virtual void setupTextures() {
        LOG << "setup " << TextureSize << "textures";
        for (int i = 0; i < TextureSize; ++i) {
            texture[i] = Proto::Textures[i]();
            LOG << "slot " << i << ": " << texture[i];
        }
    }

    virtual void bindAttribute(int index) {
        const auto &proto = Proto::Attributes[index];
        if (proto.integer) {
            glVertexAttribIPointer(attribute[index], proto.size, proto.type,
                                   proto.stride, proto.offset);
        } else {
            glVertexAttribPointer(attribute[index], proto.size, proto.type,
                                  proto.normalized, proto.stride, proto.offset);
        }
    }

    virtual void bindBuffer(GLuint buf) {
        glBindBuffer(GL_ARRAY_BUFFER, buf);
        for (int i = 0; i < AttribSize; ++i) {
            bindAttribute(i);
        }
    }

    virtual void bindProgram() {
        glUseProgram(program);
        glBindVertexArray(vertexArray);
    }

    virtual void bindTextures() {
        for (int i = 0; i < TextureSize; ++i) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, texture[i]);
        }
    }
public:
    virtual void setup() {
        LOG << "setup object: " << ProgramBase<Proto>::Name;
        setupProgram();
        glUseProgram(program);
        setupArray();
        glBindVertexArray(vertexArray);
        setupBuffers();
        setupAttributes();
        setupUniforms();
        setupTextures();
    }

    virtual void reset() {
        LOG << "reset object: " << ProgramBase<Proto>::Name;
        LOG << "delete array: " << vertexArray;
        if (BufferSize) {
            LOG << "delete " << BufferSize << " buffers";
            glDeleteBuffers(BufferSize, buffer);
        }
    }
};

#endif
