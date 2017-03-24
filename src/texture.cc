#include "texture.hpp"
#include "util.hpp"
#include <png.h>
#include <cstdio>

static const int PNG_BYTES_TO_CHECK = 4;

GLuint Texture::loadTexture(const char *name) {
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
        for (int j = 0; j < width; ++j) {
            unsigned char *base = rows[i] + 4 * j;
            for (int k = 0; k < 4; ++k) {
                data[n++] = base[k];
            }
        }
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    LOG << "generate texture: " << name << " -> " << texture;

    png_destroy_read_struct(&png, &info, 0);
    delete []data;
    fclose(fp);
    return texture;
}

static GLuint texSmall = 0;
GLuint Texture::small() {
    if (!texSmall) texSmall = loadTexture("assets/small.png");
    return texSmall;
}
