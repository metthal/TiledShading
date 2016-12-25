#ifndef TILED_SHADING_SCENE_CAMERA_H
#define TILED_SHADING_SCENE_CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	Camera(const glm::vec3& position, const glm::vec3& lookAt, float fov, float aspectRatio);

	const glm::vec3& getPosition() const;
	const glm::vec3& getDirection() const;

	void lookAt(const glm::vec3& position);
	void setPosition(const glm::vec3& position);
	void setDirection(const glm::vec3& direction);
	void setFieldOfView(float fov);
	void setAspectRatio(std::uint32_t width, std::uint32_t height);
	void setAspectRatio(float aspectRatio);

	glm::mat4 getViewTransform() const;

	void moveForwards(std::uint32_t diff);
	void moveBackwards(std::uint32_t diff);
	void strafeLeft(std::uint32_t diff);
	void strafeRight(std::uint32_t diff);
	void turnLeft(std::uint32_t diff, float angle);
	void turnRight(std::uint32_t diff, float angle);
	void turnUp(std::uint32_t diff, float angle);
	void turnDown(std::uint32_t diff, float angle);

private:
	glm::vec3 _position;
	glm::vec3 _direction;
	float _fov;
	float _aspectRatio;
	float _moveSpeed;
	float _turnSpeed;
};

#endif