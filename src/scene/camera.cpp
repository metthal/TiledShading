#include <glm/ext.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "scene/camera.h"

Camera::Camera() : Camera({0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 60.0f, 4.0f / 3.0f)
{
}

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

void Camera::lookAt(const glm::vec3& position)
{
	_direction = glm::normalize(position - _position);
}

void Camera::setPosition(const glm::vec3& position)
{
	_position = position;
}

void Camera::setDirection(const glm::vec3& direction)
{
	_direction = glm::normalize(direction);
}

void Camera::setFieldOfView(float fov)
{
	_fov = fov;
}

void Camera::setAspectRatio(std::uint32_t width, std::uint32_t height)
{
	setAspectRatio(static_cast<float>(width) / height);
}

void Camera::setAspectRatio(float aspectRatio)
{
	_aspectRatio = aspectRatio;
}


glm::mat4 Camera::getViewTransform() const
{
	return glm::perspective(glm::radians(_fov), _aspectRatio, 0.1f, 1000.0f) * glm::lookAt(_position, _position + _direction, glm::vec3(0.0, 1.0f, 0.0f));
}

void Camera::moveForwards(float step)
{
	_position += step * _direction;
}

void Camera::moveBackwards(float step)
{
	_position -= step * _direction;
}

void Camera::strafeLeft(float step)
{
	auto right = glm::normalize(glm::cross(_direction, glm::vec3(0.0, 1.0f, 0.0f)));
	_position -= step * right;
}

void Camera::strafeRight(float step)
{
	auto right = glm::normalize(glm::cross(_direction, glm::vec3(0.0, 1.0f, 0.0f)));
	_position += step * right;
}

void Camera::turnLeft(float angle)
{
	auto right = glm::normalize(glm::cross(_direction, glm::vec3(0.0, 1.0f, 0.0f)));
	auto up = glm::normalize(glm::cross(_direction, right));
	_direction = glm::rotate(_direction, angle, up);
}

void Camera::turnRight(float angle)
{
	auto right = glm::normalize(glm::cross(_direction, glm::vec3(0.0, 1.0f, 0.0f)));
	auto up = glm::normalize(glm::cross(_direction, right));
	_direction = glm::rotate(_direction, -angle, up);
}

void Camera::turnUp(float angle)
{
	auto right = glm::normalize(glm::cross(_direction, glm::vec3(0.0, 1.0f, 0.0f)));
	_direction = glm::rotate(_direction, angle, right);
}

void Camera::turnDown(float angle)
{
	auto right = glm::normalize(glm::cross(_direction, glm::vec3(0.0, 1.0f, 0.0f)));
	_direction = glm::rotate(_direction, -angle, right);
}