#include <GLFW/glfw3native.h>
#include <Windows.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

#include "shaders/Shader.h"
#include "io/camera.h"
#include "graphics/model.h"
#include "graphics/button.h"

#include <iostream>
#include "lighting/sun.h"
#include "graphics/skybox.h"
#include "io/mouse.h"
#include "instances/instancing.h"
#include "graphics/textures.h"

/*
* 
* This was made by marsh on GitHub
* https://github.com/marsh-github
* 
* This program has lots of work and is heavily unfinished
* Feel free to fork it or tweak it
* 
*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, double dt);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void showMenuCallback();

bool rightMouseButtonPressed = false;
double lastMouseX, lastMouseY;

double scrollDy = 0;
double getScrollDy() {
	double _scrollDy = scrollDy;
	scrollDy = 0;
	return _scrollDy;
}

int SCR_WIDTH = 1920;
int SCR_HEIGHT = 1080;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float x, y, z;

bool firstMouse = true;

float yaw = -90.0f;
float pitch = 0.0f;

double lockedX = 0.0;
double lockedY = 0.0;

double mXoffset, mYoffset;

glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
Sun sun1;

bool isFreeCamMode = true;

int button1;



int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Render", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	stbi_set_flip_vertically_on_load(false);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	ImFont* comicSans = io.Fonts->AddFontFromFileTTF("assets/fonts/comic.ttf", 16.0f);
	ImFont* calibri = io.Fonts->AddFontFromFileTTF("assets/fonts/calibri.ttf", 16.0f); // example font
	ImGui::StyleColorsLight();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	Mouse mouse;
	GLFWcursor* customCursor = mouse.createCustomCursor("assets/slick_arrow-delta.png");
	glfwSetCursor(window, customCursor);

	Shader ourShader("assets/object.vs", "assets/object.fs");

	int model1 = instances.New("Model", { "assets/Tree/Tree.obj" });
	int model2 = instances.New("Model", { "assets/Source/robloxdefaultavatar.obj" });

	button1 = instances.New("Button", { "Import OBJ", ImVec2(5, 30), ImVec2(95, 25) });
	buttonList[button1].SetFont(comicSans);
	buttonList[button1].SetVisibility(false);
	buttonList[button1].SetClickCallback([] {
		OPENFILENAME ofn;
		char fileName[MAX_PATH] = "";
		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = GetActiveWindow();
		ofn.lpstrFilter = "Model Files (*.obj)\0*.obj\0";
		ofn.lpstrFile = fileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = "";

		string fileNameStr;

		if (GetOpenFileName(&ofn))
			fileNameStr = fileName;

		instances.New("Model", { fileNameStr.c_str() });

		});
	int button2 = instances.New("Button", { "File", ImVec2(0, 0), ImVec2(100, 30) });
	buttonList[button2].SetFont(comicSans);
	buttonList[button2].SetTransparency(0.7f);
	buttonList[button2].SetClickCallback(showMenuCallback);

	std::vector<std::string> faces{
	"assets/bluecloud_rt.bmp",
	"assets/bluecloud_lf.bmp",
	"assets/bluecloud_up.bmp",
	"assets/bluecloud_dn.bmp",
	"assets/bluecloud_ft.bmp",
	"assets/bluecloud_bk.bmp"
	};

	Skybox skybox(faces);

	while (!glfwWindowShouldClose(window))
	{
		float currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		processInput(window, deltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ourShader.use();
		ourShader.setVec3("viewPos", camera.cameraPos);

		sun1.setTimeOfDay(17.0f);
		sun1.applyToShader(ourShader);

		/* pointlight properties
		ourShader.setVec3("pLight.position", lightPos);
		ourShader.setVec3("pLight.ambient", 0.2f, 0.2f, 0.2f);
		ourShader.setVec3("pLight.diffuse", 0.5f, 0.5f, 0.5f);
		ourShader.setVec3("pLight.specular", 1.0f, 1.0f, 1.0f);
		ourShader.setFloat("pLight.constant", 1.0f);
		ourShader.setFloat("pLight.linear", 0.09f);
		ourShader.setFloat("pLight.quadratic", 0.032f);
		*/

		glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_HEIGHT);

		float aspectRatio = static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT);
		glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), aspectRatio, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.00f, 1.00f, 1.00f));
		ourShader.setMat4("model", model);

		// Render all models
		for (auto it = modelsList.begin(); it != modelsList.end(); ++it) {
			Model& item = *it;
			item.Draw(ourShader);
		}

		// Render all buttons
		for (auto it = buttonList.begin(); it != buttonList.end(); ++it) {
			Button& item = *it;
			item.Render();
		}

		skybox.draw(view, projection);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyCursor(customCursor);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

bool active = false;

void showMenuCallback() {
	if (active == false) {
		buttonList[button1].SetVisibility(true);
		active = true;
	}
	else {
		buttonList[button1].SetVisibility(false);
		active = false;
	}
}

void processInput(GLFWwindow* window, double dt)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.updateCameraPos(CameraDirection::FORWARD, dt);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.updateCameraPos(CameraDirection::LEFT, dt);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.updateCameraPos(CameraDirection::BACKWARD, dt);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.updateCameraPos(CameraDirection::RIGHT, dt);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera.updateCameraPos(CameraDirection::UP, dt);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		camera.updateCameraPos(CameraDirection::DOWN, dt);
	}
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset > 0) {
		camera.updateCameraPos(CameraDirection::FORWARD, 1);
	}
	if (yoffset < 0) {
		camera.updateCameraPos(CameraDirection::BACKWARD, 1);
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (rightMouseButtonPressed) {

		double xoffset = xpos - lockedX;
		double yoffset = lockedY - ypos;

		camera.updateCameraDirection(xoffset, yoffset, 0.1f);

		glfwSetCursorPos(window, lockedX, lockedY);
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			rightMouseButtonPressed = true;
			glfwGetCursorPos(window, &lockedX, &lockedY);
		}
		else if (action == GLFW_RELEASE) {
			rightMouseButtonPressed = false;
		}
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow)
{
	main();
	return 0;
}