#include "Application.h"

Camera* Application::camera = nullptr;
Shader Application::skyboxShader;
Shader Application::mapShader;
Shader Application::terrainShader;
Shader Application::kartShader;

void Application::Run()
{
	if (!InitWindow())
	{
		std::cout << "Couldn't initialize GLFW window !" << std::endl;
		return;
	}

	camera = new Camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(-10.0f, 11.0f, -45.0f));

	std::filesystem::path localPath = std::filesystem::current_path();
	std::string resourcesFolder = localPath.string() + "\\Resources";

	Skybox skybox(resourcesFolder, skyboxShader);
	Raceway raceway(resourcesFolder, mapShader);
	Terrain terrain(resourcesFolder, terrainShader);
	Kart kart(resourcesFolder, kartShader); //TODO: 

	Render(skybox, raceway, terrain, kart);
}

Application::~Application()
{
	mapShader.Delete();
	skyboxShader.Delete();
	kartShader.Delete();
	glfwTerminate();

	delete camera;
}

bool Application::InitWindow()
{
	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Karting Prejmer", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewInit();
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);
	glfwSetKeyCallback(window, KeyCallback);

	glfwSetCursorPos(window, static_cast<double>(SCR_WIDTH) / 2, static_cast<double>(SCR_HEIGHT) / 2);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

void keyInput(GLFWwindow* window, Kart& kart)
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		kart.ProcessKeyboard(K_FORWARD, 0.04f);
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		kart.ProcessKeyboard(K_LEFT, 0.04f);
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		kart.ProcessKeyboard(K_RIGHT, 0.04f);
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		kart.ProcessKeyboard(K_BACKWARD, 0.04f);
}

void Application::SetupShadowMapping()
{
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	// Shadow shader
	//shadowShader = Shader("shadow_vertex.glsl", "shadow_fragment.glsl");

	// Depth map framebuffer
	glGenFramebuffers(1, &depthMapFBO);
	// Generate depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// Attach depth texture to framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Application::Render(Skybox& skybox, Raceway& raceway, Terrain& terrain, Kart& kart)
{
	while (!glfwWindowShouldClose(window))
	{
		keyInput(window, kart);

		// Per-frame time logic
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Lighting
		float clearR = 0.07f + skyLight / 2.f - 0.1f;
		float clearG = 0.13f + skyLight / 2.f - 0.1f;
		float clearB = 0.17 + skyLight / 2.f - 0.1f;
		glClearColor(clearR, clearG, clearB, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Input
		ProcessInput(window, camera, deltaTime);

		//TODO: REDENDER TWICE

		// Render here
		raceway.Render(camera, mapShader);//TODO: IN LOC DE CAMERA DA-I MATRICEA DE PROIECTIE PE BAZA POZITIEI SURSEI DE LUMINA SI DIRECTIEI
		terrain.Render(camera, terrainShader); //TODO: MATRICEA DE VIEW SI MATRICEA DE PROIECTIE IN LOC DE CAMERA
		kart.Render(camera, mapShader); //TODO: TWO SHADERS PER OBJ ONE FOR LIGHTSOURCE AND THE OTHER FOR 
		skybox.Render(camera, skyboxShader);

	    glm::vec3 kartPosition = kart.GetPosition();
		camera->UpdatePosition(kartPosition);

		// Swap front and back buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Application::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	camera->Reshape(width, height);
}

void Application::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	camera->MouseControl((float)xpos, (float)ypos);
}

void Application::ScrollCallback(GLFWwindow* window, double xoffset, double yOffset)
{
	camera->ProcessMouseScroll((float)yOffset);
}

void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (tolower(key) == 'i' && lightIntensityValue.x < 0.9f)
	{
		lightIntensityValue += glm::vec3(0.05f);
	}

	if (tolower(key) == 'o' && lightIntensityValue.x > 0.2f)
	{
		lightIntensityValue -= glm::vec3(0.05f);
	}

	skyboxShader.Use();
	skyboxShader.SetVec3("lightColor", lightIntensityValue);
	mapShader.Use();
	mapShader.SetVec3("lightColor", lightIntensityValue);
	terrainShader.Use();
	terrainShader.SetVec3("lightColor", lightIntensityValue);

	skyLight = lightIntensityValue.x;
}