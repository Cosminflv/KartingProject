#include "Terrain.h"

Terrain::Terrain(const std::string& resourcesFolder, Shader& terrainShader) : map("plane.obj")
{
	InitMap(resourcesFolder, terrainShader);

	terrainShader.Use();
	terrainShader.SetVec3("lightColor", glm::vec3(0.6f, 0.6f, 0.6f));
}

void Terrain::Render(Camera* pCamera, Shader& terrainShader)
{
	terrainShader.Use();
	pCamera->UpdateCameraVectors();
	pCamera->Use(terrainShader);

	glBindTexture(GL_TEXTURE_2D, terrainTexture);

	// Render multiple instances of the terrain
	for (float x = -500.0f; x <= 500.0f; x += 60.0f) { // Adjust the range and step size as needed
		for (float z = -500.0f; z <= 500.0f; z += 60.0f) { // Adjust the range and step size as needed
			map.SetPosition(glm::vec3(x, 7.0f, z)); // Set the position of the terrain mesh
			map.Render(terrainShader); // Render the terrain mesh
		}
	}
}

void Terrain::InitMap(const std::string& resourcesFolder, Shader& terrainShader)
{
	std::vector<GLuint> indices = {
	0, 1, 2, // First triangle
	0, 2, 3  // Second triangle
	};

	terrainShader.Set("Texture.shader");

	terrainTexture = CreateTexture(resourcesFolder + "\\grass2.jpg");
	terrainShader.SetInt("plane", 1);

	map.SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
	map.SetPosition(glm::vec3(0.0f, 7.0f, 0.0f));
	map.InitIndices(indices);
	map.InitVAO();
}