#include "Kart.h"

Kart::Kart(const std::string& resourcesFolder, Shader& kartShader) : kartMesh("kart.obj")
{
	InitKart(resourcesFolder, kartShader);

	kartShader.Use();
	kartShader.SetVec3("lightColor", glm::vec3(0.7f, 0.7f, 0.0f));

	MovementSpeed = 1.50f;
	TurningSpeed = 1.0f;
	WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	updateFront();
	m_listener = nullptr;
}

void Kart::Render(Camera* camera, Shader& kartShader)
{
	kartShader.Use();
	camera->UpdateCameraVectors();
	camera->Use(kartShader);
	glBindTexture(GL_TEXTURE_2D, kartTexture);
	kartMesh.Render(kartShader);
}

static glm::vec3 initialForwardDirection = glm::vec3(0.0f, 0.0f, -1.0f);


void Kart::ProcessKeyboard(Direction direction, float deltaTime)
{	
	glm::vec3 movementDirection = glm::vec3(0.0f);
	float targetYawRotation = 0.0f;
	const float maxSteerAngle = 30.0f; // Maximum steering angle

	if (direction == K_LEFT)
	{
		targetYawRotation = maxSteerAngle; // Steer to the left
	}
	else if (direction == K_RIGHT)
	{
		targetYawRotation = -maxSteerAngle; // Steer to the right
	}

	// Calculate the current forward direction of the kart based on its rotation
	float rotationRad = glm::radians(kartMesh.GetRotation().y);
	glm::vec3 forwardDirection(sin(rotationRad), 0.0f, cos(rotationRad));

	// Apply rotation only when steering (left or right)
	if (direction == K_LEFT || direction == K_RIGHT)
	{
		// Smoothly adjust the kart's steering angle towards the target steering angle
		const float maxSteerSpeed = 30.0f; // Maximum steering speed in degrees per second
		float steerAmount = glm::clamp(targetYawRotation, -maxSteerSpeed * deltaTime, maxSteerSpeed * deltaTime);
		float currentYawRotation = kartMesh.GetRotation().y;
		float newSteerRotation = currentYawRotation + steerAmount;
		kartMesh.SetRotation(glm::vec3(0.0f, newSteerRotation, 0.0f));

		// Update last forward position based on the kart's rotation
		float newRotationRad = glm::radians(newSteerRotation);
		float forwardX = -sin(newRotationRad); // Calculate x component of forward direction
		float forwardZ = -cos(newRotationRad); // Calculate z component of forward direction
		forwardDirection = glm::normalize(glm::vec3(forwardX, 0.0f, forwardZ)); // Normalize the forward direction

		if (accelerate)
		{
			direction = accelerateDir == 1 ? K_FORWARD : K_BACKWARD;
			accelerate = false;
		}
	}

	if (direction == K_FORWARD)
	{
		accelerate = true;

		// Smoothly adjust the kart's position towards the target position
		float acceleration = 5.0f; // Adjust this value to control the acceleration
		glm::vec3 currentPosition = kartMesh.GetPosition();

		// Calculate velocity vector
		glm::vec3 velocity = forwardDirection * acceleration;

		// Calculate new position based on velocity
		glm::vec3 newPosition = currentPosition + velocity * deltaTime;

		// Adjust the position to ensure movement along the Z-axis
		//newPosition.x = currentPosition.x; // Retain the current x-axis position

		kartMesh.SetPosition(newPosition);

		accelerateDir = -1;
		// Update last forward position
		//forwardDirection = newPosition;
	}
	else if (direction == K_BACKWARD)
	{
		accelerate = true;
		// Smoothly adjust the kart's position towards the target position
		float acceleration = 5.0f; // Adjust this value to control the acceleration
		glm::vec3 currentPosition = kartMesh.GetPosition();

		// Calculate velocity vector
		glm::vec3 velocity = -forwardDirection * acceleration; // Note the negative sign

		// Calculate new position based on velocity
		glm::vec3 newPosition = currentPosition + velocity * deltaTime;

		// Adjust the position to ensure movement along the Z-axis
		//newPosition.x = currentPosition.x; // Retain the current x-axis position

		kartMesh.SetPosition(newPosition);

		//accelerateDir = 1;
	}

	//updateFront();
	auto coordonates = kartMesh.GetPosition();
	if (m_listener != nullptr)
		m_listener->OnKartPositionChanged(coordonates.x, coordonates.y, coordonates.z);
}

inline void Kart::AddListener(IKartListener* listener)
{
	m_listener = listener;
}

inline void Kart::RemoveListener()
{
	m_listener = nullptr;
}

void Kart::InitKart(const std::string& resourcesFolder, Shader& kartShader)
{
	kartShader.Set("Texture.shader");

	kartTexture = CreateTexture(resourcesFolder + "\\kart.jpg");
	kartShader.SetInt("kart", 2);

	kartMesh.SetScale(glm::vec3(0.05f, 0.05f, 0.05f));
	kartMesh.SetPosition(glm::vec3(-10.0f, 8.0f, -45.0f));
	//kartMesh.SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	kartMesh.InitVAO();
}

void Kart::updateFront()
{
	float yawRad = glm::radians(Yaw);

	// Calculate front direction based on yaw rotation
	glm::vec3 front(-sin(yawRad), 0.0f, -cos(yawRad));
	Front = glm::normalize(front);

	// Update right direction using cross product
	Right = glm::normalize(glm::cross(Front, WorldUp));
}
