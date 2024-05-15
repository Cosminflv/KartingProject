#pragma once
#include "Mesh.h"
#include "Camera.h"
#include "TextureLoader.h"

class Terrain
{
public:
	Terrain() = default;
	Terrain(const std::string& resourcesFolder, Shader& mapShader);

	void  Render(Camera* pCamera, Shader& terrainShader);

private:
	void InitMap(const std::string& resourcesFolder, Shader& mapShader);

private:
	Mesh map;
	unsigned int terrainTexture;
};

