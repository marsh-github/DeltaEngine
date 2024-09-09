#include "mouse.h"

GLFWcursor* Mouse::createCustomCursor(const char* filepath) {
    int width, height, channels;

    unsigned char* data = stbi_load(filepath, &width, &height, &channels, 4);

    if (!data) {
        std::cerr << "Failed to load cursor image: " << filepath << std::endl;
        return nullptr;
    }

    GLFWimage image;
    image.width = width;
    image.height = height;
    image.pixels = data;

    int hotspotX = width / 2;
    int hotspotY = height / 2;

    GLFWcursor* cursor = glfwCreateCursor(&image, hotspotX, hotspotY);

    stbi_image_free(data);

    return cursor;
}