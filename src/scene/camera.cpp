#include <glm/ext.hpp>

#include "scene/camera.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& lookAt, float fov, float aspectRatio)
	: _position(position), _direction(glm::normalize(lookAt - position)), _fov(fov), _aspectRatio(aspectRatio)
{
}

const glm::vec3& Camera::getPosition() const
{
	return _position;
}

const glm::vec3& Camera::getDirection() const
{
	return _direction;
}

glm::mat4 Camera::getViewTransform() const
{
	return glm::perspective(_fov, _aspectRatio, 0.1f, 100.0f) * glm::lookAt(_position, _position + _direction, glm::vec3(0.0, 1.0f, 0.0f));
}