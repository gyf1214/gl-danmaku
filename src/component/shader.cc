#include "../ext.hpp"
#include "component/shader.hpp"

using namespace std;

vector<GLuint> programs;

GLuint Shader::compileShader(GLenum type, const char *src) {
    LOG << "load shader:\n" << src;

    GLuint shader = glCreateShader(type);
    int length = strlen(src);
    glShaderSource(shader, 1, &src, &length);
    glCompileShader(shader);

    int success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        GLchar *logStr = new GLchar[logLength];
        glGetShaderInfoLog(shader, logLength, NULL, logStr);

        FAIL << "compile shader failed:\n" << logStr;
    }

    return shader;
}

void Shader::linkProgram(GLuint program) {
    LOG << "link program: " << program;
    glLinkProgram(program);
    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        GLint logLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        GLchar *logStr = new GLchar[logLength];
        glGetProgramInfoLog(program, logLength, NULL, logStr);

        FAIL << "link program " << program << " failed:\n" << logStr;
    }
    programs.push_back(program);
}

void Shader::release() {
    LOG << "release global shaders";
    int n = programs.size();
    for (int i = 0; i < n; ++i) {
        LOG << "delete program: " << programs[i];
        glDeleteProgram(programs[i]);
    }
    programs.clear();
}

GLuint Shader::programShader(const char *vshSrc, const char *fshSrc) {
    GLuint program = glCreateProgram();
    LOG << "load program: " << program;

    GLuint vsh = compileShader(GL_VERTEX_SHADER, vshSrc);
    GLuint fsh = compileShader(GL_FRAGMENT_SHADER, fshSrc);
    glAttachShader(program, vsh);
    glAttachShader(program, fsh);

    linkProgram(program);

    glDetachShader(program, vsh);
    glDetachShader(program, fsh);
    glDeleteShader(vsh);
    glDeleteShader(fsh);

    return program;
}
