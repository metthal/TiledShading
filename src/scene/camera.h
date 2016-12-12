#ifndef TILED_SHADING_SCENE_CAMERA_H
#define TILED_SHADING_SCENE_CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(const glm::vec3& position, const glm::vec3& lookAt, float fov = 60.0f, float aspectRatio = 4.0f/3.0f);

	const glm::vec3& getPosition() const;
	const glm::vec3& getDirection() const;

	glm::mat4 getViewTransform() const;

private:
	glm::vec3 _position;
	glm::vec3 _direction;
	float _fov;
	float _aspectRatio;
};

#endif