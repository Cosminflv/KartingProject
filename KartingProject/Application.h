#pragma once
#include <GL/glew.h>
#include <glfw3.h>
#include <iostream>

#include "Camera.h"

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

static double deltaTime = 0.0f;
static double lastFrame = 0.0f;

extern GLuint VaoId, VboId, IboId, ColorBufferId, VertexShaderId, FragmentShaderId, ProgramId;
extern GLuint ProjMatrixLocation, ViewMatrixLocation, WorldMatrixLocation;

// Shader-ul de varfuri / Vertex shader (este privit ca un sir de caractere)
extern const GLchar* VertexShader;
// Shader-ul de fragment / Fragment shader (este privit ca un sir de caractere)
extern const GLchar* FragmentShader;

class Application
{
public:
	Application() = default;
	~Application();

	void Run();

private:

	bool InitWindow();
	void Initialize();

	void CreateVBO();
	void CreateShaders();

	static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yOffset);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void RenderFrame();
	void RenderCube();

	void Cleanup();

	void DestroyShaders();
	void DestroyVBO();

private:
	GLFWwindow* window;
	static Camera* camera;

};

