#pragma once
#include "Mesh.h"
#include "Camera.h"
#include "TextureLoader.h"

class Raceway
{
public:
	Raceway() = default;
	Raceway(const std::string& resourcesFolder, Shader& mapShader);

	void Render(Camera* pCamera, const glm::mat4& lightSpaceMatrix, Shader& mapShader);

private:
	void InitMap(const std::string& resourcesFolder, Shader& mapShader);

private:
	Mesh map;
	unsigned int mapTexture;
};