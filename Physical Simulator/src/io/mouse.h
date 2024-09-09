#pragma once

#include <Windows.h>
#include <stb/stb_image.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Mouse {
public:
	GLFWcursor* createCustomCursor(const char* filepath);
};