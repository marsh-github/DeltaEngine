#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include "../shaders/Shader.h"

class Skybox {
public:
    Skybox(const std::vector<std::string>& faces);
    void draw(const glm::mat4& view, const glm::mat4& projection);

private:
    unsigned int cubemapTexture;
    unsigned int skyboxVAO, skyboxVBO;
    Shader skyboxShader;

    unsigned int loadCubemap(const std::vector<std::string>& faces);
    void setupSkybox();
};

#endif