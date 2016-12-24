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

	void moveForwards(float step);
	void moveBackwards(float step);
	void strafeLeft(float step);
	void strafeRight(float right);
	void turnLeft(float angle);
	void turnRight(float angle);
	void turnUp(float angle);
	void turnDown(float angle);

private:
	glm::vec3 _position;
	glm::vec3 _direction;
	float _fov;
	float _aspectRatio;
};

#endif