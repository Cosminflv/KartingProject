#pragma once
#pragma once
#include "Mesh.h"
#include "Camera.h"
#include "TextureLoader.h"

class Lamp
{
public:
	Lamp() = default;
	Lamp(const std::string& resourcesFolder, Shader& mapShader);

	void Render(Camera* pCamera, Shader& mapShader);

private:
	void InitLamp(const std::string& resourcesFolder, Shader& mapShader);

private:
	Mesh lamp;
};
