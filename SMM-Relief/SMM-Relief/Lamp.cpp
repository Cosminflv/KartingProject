#include "Lamp.h"

Lamp::Lamp(const std::string& resourcesFolder, Shader& mapShader) : lamp("lamp.obj")
{
	InitLamp(resourcesFolder, mapShader);

	mapShader.Use();
	mapShader.SetVec3("lightColor", glm::vec3(0.6f, 0.6f, 0.6f));
}

void Lamp::Render(Camera* pCamera, Shader& mapShader)
{
	mapShader.Use();
	pCamera->UpdateCameraVectors();
	pCamera->Use(mapShader);

	
	lamp.Render(mapShader);
}

void Lamp::InitLamp(const std::string& resourcesFolder, Shader& mapShader)
{
	mapShader.Set("Basic.shader");

	mapShader.SetInt("lamp", 1);

	lamp.SetScale(glm::vec3(5.2f, 5.2f, 5.2f));
	lamp.SetPosition(glm::vec3(0.0f, 5.0f, 0));
	lamp.InitVAO();
}
