#include "texture.hpp"
#include "util.hpp"
#include <png.h>
#include <cstdio>

static const int PNG_BYTES_TO_CHECK = 4;

GLuint Texture::loadTexture(const char *name, GLint wrap) {
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

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    LOG << "generate texture: " << name << " -> " << texture;

    png_destroy_read_struct(&png, &info, 0);
    delete []data;
    fclose(fp);
    return texture;
}

#define defineTexture(name, path, wrap) static GLuint tex_##name = 0;\
GLuint Texture::name() {\
    if (!tex_##name) tex_##name = loadTexture(path, wrap);\
    return tex_##name;\
}

defineTexture(small, "assets/small.png", GL_CLAMP);
defineTexture(etama, "assets/etama.png", GL_CLAMP);
defineTexture(rockNormal, "assets/rock_normal.png", GL_REPEAT);
defineTexture(sceneWall, "assets/scene_wall4.png", GL_REPEAT);
defineTexture(sceneFloor, "assets/lava2.png", GL_CLAMP);
