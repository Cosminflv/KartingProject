#include "Raceway.h"

Raceway::Raceway(const std::string& resourcesFolder, Shader& mapShader) : map("raceway.obj")
{
	InitMap(resourcesFolder, mapShader);

	mapShader.Use();
	mapShader.SetVec3("lightColor", glm::vec3(0.6f, 0.6f, 0.6f));
}

void Raceway::Render(Camera* pCamera, const glm::mat4& lightSpaceMatrix, Shader& mapShader)
{
	mapShader.Use();
	mapShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
	pCamera->UpdateCameraVectors();
	pCamera->Use(mapShader);

	glBindTexture(GL_TEXTURE_2D, mapTexture);
	map.Render(mapShader);
}

void Raceway::InitMap(const std::string& resourcesFolder, Shader& mapShader)
{
	mapShader.Set("Map.shader");

	mapTexture = CreateTexture(resourcesFolder + "\\MapTexture\\circuit.jpg");
	mapShader.SetInt("racetrack", 1);

	map.SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
	map.SetPosition(glm::vec3(0.0f));
	map.InitVAO();
}
