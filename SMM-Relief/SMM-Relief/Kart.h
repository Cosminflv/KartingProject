#pragma once
#include "Mesh.h"
#include "Camera.h"
#include "TextureLoader.h"

enum Direction {
	K_FORWARD,
	K_BACKWARD,
	K_LEFT,
	K_RIGHT
};

class Kart
{
public:
	Kart() = default;
	Kart(const std::string& resourcesFolder, Shader& kartShader);
	const glm::vec3& GetPosition() const { return kartMesh.GetPosition(); }
	void Render(Camera* camera, Shader& kartShader);

	void ProcessKeyboard(Direction direction, float deltaTime);
	void AddListener(IKartListener* listener);
	void RemoveListener();
private:
	void InitKart(const std::string& resourcesFolder, Shader& kartShader);
	void updateFront();

private:
	Mesh kartMesh;
	unsigned int kartTexture;

	glm::vec3 Front;
	glm::vec3 Right;
	glm::vec3 Up;
	glm::vec3 WorldUp;
	float Yaw;

	float MovementSpeed;
	float TurningSpeed;

	float rotation = 0.0f;
	float accelerateDir;
	bool accelerate = false;
	IKartListener* m_listener;
};

