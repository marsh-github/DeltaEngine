#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

struct DirectionalLight {
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

class Sun {
public:
    DirectionalLight light;

    Sun() {
        // Initialize with default "noon" values
        light.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
        light.ambient = glm::vec3(0.3f, 0.3f, 0.3f);
        light.diffuse = glm::vec3(1.0f, 0.95f, 0.8f);
        light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    }

    void setTimeOfDay(float hour) {
        if (hour < 0.0f) hour = 0.0f;
        if (hour > 24.0f) hour = 24.0f;

        if (hour >= 6.0f && hour <= 18.0f) {
            float t = (hour - 6.0f) / 12.0f;
            light.direction = glm::vec3(glm::mix(-0.5f, 0.5f, t), -glm::mix(1.0f, 0.1f, t), -0.3f);
            light.diffuse = glm::vec3(glm::mix(0.9f, 1.0f, t), glm::mix(0.5f, 0.95f, t), glm::mix(0.3f, 0.8f, t));
            light.ambient = glm::vec3(glm::mix(0.4f, 0.3f, t), glm::mix(0.3f, 0.3f, t), glm::mix(0.2f, 0.3f, t));
        }
        else {
            // Nighttime (sun below horizon)
            light.direction = glm::vec3(0.0f, -1.0f, 0.0f);
            light.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
            light.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
        }
    }
    
    glm::vec3 getDirection() {
        return light.direction;
    }

    void applyToShader(const Shader& shader) {
        shader.use();
        shader.setVec3("dirLight.direction", light.direction);
        shader.setVec3("dirLight.ambient", light.ambient);
        shader.setVec3("dirLight.diffuse", light.diffuse);
        shader.setVec3("dirLight.specular", light.specular);
    }
};