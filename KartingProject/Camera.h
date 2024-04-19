#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <glm.hpp>

enum class EMovementType
{
	UNKNOWN,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera
{
private:
    // Default camera values
    const float zNEAR = 0.1f;
    const float zFAR = 500.f;
    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float FOV = 45.0f;
    glm::vec3 startPosition;

public:
    Camera(const int width, const int height, const glm::vec3& position);
    ~Camera() = default;

    void Reset(int width, int height);
    void Set(const int width, const int height, const glm::vec3& position);

    // GETTERS
    const glm::mat4 GetViewMatrix() const;
    const glm::mat4 GetProjectionMatrix() const;


    // PUBLIC METHODS
    void MouseControl(float xpos, float ypos);
    void ProcessKeyboard(EMovementType direction, float deltaTime);
    void ProcessMouseScroll(float yOffset);

	void Reshape(int windowWidth, int windowHeight);

	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
	void UpdateCameraVectors();

protected:
    const float cameraSpeedFactor = 300.0f; //FA L MAI MARE SA SE MISTE MAI REPEDE
    const float mouseSensitivity = 0.1f;

    // Perspective properties
    float zNear;
    float zFar;
    float FoVy;
    int width;
    int height;
    bool isPerspective;

    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 worldUp;

    // Euler Angles
    float yaw;
    float pitch;

    bool bFirstMouseMove = true;
    float lastX = 0.f, lastY = 0.f;
};