#ifndef __COMMON
#define __COMMON

#include <cstdlib>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <png.h>

#define Offset(type, member)    ((void *)(&(((type *)0)->member)))
#define Alias(src, tar)         inline decltype(src) &tar() { return tar; }

using glm::vec2;
using glm::vec3;
using glm::vec4;

class Scene;
class Renderer;
class Transformer;

#endif
