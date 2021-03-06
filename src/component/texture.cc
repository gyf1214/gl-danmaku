#include "../ext.hpp"
#include "component/texture.hpp"

using namespace std;

static const int PNG_BYTES_TO_CHECK = 4;

static vector<GLuint> texes;

GLuint Texture::genTexture(GLuint inter, GLuint wraps, GLuint wrapt) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wraps);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapt);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, inter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, inter);
    return texture;
}

GLuint Texture::loadTexture(const char *name, GLuint wraps, GLuint wrapt) {
    FILE *fp;
    char buf[PNG_BYTES_TO_CHECK];

    LOG << "read png " << name;

    CHECK(fp = fopen(name, "rb")) << "open " << name << " failed!";
    int cnt = fread(buf, 1, PNG_BYTES_TO_CHECK, fp);
    CHECK(cnt == PNG_BYTES_TO_CHECK &&
          !png_sig_cmp((png_bytep)buf, 0, PNG_BYTES_TO_CHECK))
          << "read " << name << " header failed!";

    rewind(fp);
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                             NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);

    CHECK(!setjmp(png_jmpbuf(png))) << "read " << name << " data failed!";
    png_init_io(png, fp);
    png_read_png(png, info, PNG_TRANSFORM_EXPAND, NULL);
    CHECK_EQ(png_get_color_type(png, info), PNG_COLOR_TYPE_RGB_ALPHA)
             << "only support RGBA png texture!";

    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    png_bytep *rows = png_get_rows(png, info);

    LOG << "read png " << name << " done: " << width << "x" << height;

    unsigned char *data = new unsigned char[width * height * 4];
    int n = 0;

    for (int i = 0; i < height; ++i) {
        unsigned char *base = rows[height - i - 1];
        for (int j = 0; j < width * 4; ++j) {
            data[n++] = base[j];
        }
    }

    GLuint texture = genTexture(GL_LINEAR, wraps, wrapt);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data);

    LOG << "generate texture: " << name << " -> " << texture;

    png_destroy_read_struct(&png, &info, 0);
    delete []data;
    fclose(fp);
    return texture;
}

void Texture::release() {
    LOG << "reset global textures";
    int n = texes.size();
    for (int i = 0; i < n; ++i) {
        LOG << "delete texture: " << texes[i];
        glDeleteTextures(1, &texes[i]);
    }
    texes.clear();
}

GLuint Texture::genDepth() {
    GLuint ret = genTexture(GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 Application::bufferWidth, Application::bufferHeight,
                 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    return ret;
}

GLuint Texture::genScreen() {
    GLuint ret = genTexture(GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
         Application::bufferWidth, Application::bufferHeight,
         0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    return ret;
}

#define defineTexture(name, path, ...) GLuint Texture::name() {\
    static GLuint tex = 0;\
    if (!tex) {\
        tex = loadTexture("assets/" path, ##__VA_ARGS__);\
        texes.push_back(tex);\
    }\
    return tex;\
}

defineTexture(small, "small.png");
defineTexture(etama, "etama.png");
defineTexture(wallNormal, "rock_normal.png", GL_REPEAT, GL_REPEAT);
defineTexture(wall, "scene_wall6.png", GL_REPEAT, GL_REPEAT);
defineTexture(lava, "lava2.png");
defineTexture(wallLava, "lava3.png", GL_MIRRORED_REPEAT, GL_CLAMP_TO_BORDER);
defineTexture(white, "white.png", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
defineTexture(trail, "trail.png");
defineTexture(wall1_ref, "wall1_ref.png", GL_MIRRORED_REPEAT, GL_REPEAT);
defineTexture(wall1_nor, "wall1_nor.png", GL_MIRRORED_REPEAT, GL_REPEAT);
defineTexture(wall1_emi, "wall1_emi.png", GL_MIRRORED_REPEAT, GL_REPEAT);
defineTexture(wall2_ref, "wall2_ref.png", GL_MIRRORED_REPEAT, GL_REPEAT);
defineTexture(wall2_nor, "wall2_nor.png", GL_MIRRORED_REPEAT, GL_REPEAT);
defineTexture(wall2_emi, "wall2_emi.png", GL_MIRRORED_REPEAT, GL_REPEAT);
defineTexture(lavaflow, "lava2.png", GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT);
