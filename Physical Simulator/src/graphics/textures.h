#ifndef TEXTURES_H
#define TEXTURES_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <stb/stb_image.h>
#include <iostream>

class Textures {
public:
    static GLuint LoadTexture(const char* filepath);
};

#endif // TEXTURELOADER_H