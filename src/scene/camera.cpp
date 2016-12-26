#include <glm/ext.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "scene/camera.h"

Camera::Camera() : Camera({0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 60.0f, 4.0f / 3.0f)
{
}

Camera::Camera(const glm::vec3& position, const glm::vec3& lookAt, float fov, float aspectRatio)
	: _position(position), _direction(glm::normalize(lookAt - position)), _fov(fov), _aspectRatio(aspectRatio), _moveSpeed(20.0f), _turnSpeed(15.0f)
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
	return glm::lookAt(_position, _position + _direction, glm::vec3(0.0, 1.0f, 0.0f));
}

glm::mat4 Camera::getProjectTransform() const
{
	return glm::perspective(glm::radians(_fov), _aspectRatio, 0.1f, 1000.0f);
}

glm::mat4 Camera::getViewProjectTransform() const
{
	return getProjectTransform() * getViewTransform();
}

void Camera::moveForwards(std::uint32_t diff)
{
	float dt = static_cast<float>(diff) / 1000.0f;
	_position += dt * _moveSpeed * _direction;
}

void Camera::moveBackwards(std::uint32_t diff)
{
	float dt = static_cast<float>(diff) / 1000.0f;
	_position -= dt * _moveSpeed * _direction;
}

void Camera::strafeLeft(std::uint32_t diff)
{
	float dt = static_cast<float>(diff) / 1000.0f;
	auto right = glm::normalize(glm::cross(_direction, glm::vec3(0.0, 1.0f, 0.0f)));
	_position -= dt * _moveSpeed * right;
}

void Camera::strafeRight(std::uint32_t diff)
{
	float dt = static_cast<float>(diff) / 1000.0f;
	auto right = glm::normalize(glm::cross(_direction, glm::vec3(0.0, 1.0f, 0.0f)));
	_position += dt * _moveSpeed * right;
}

void Camera::turnLeft(std::uint32_t diff, float angle)
{
	float dt = static_cast<float>(diff) / 1000.0f;
	auto right = glm::normalize(glm::cross(_direction, glm::vec3(0.0, 1.0f, 0.0f)));
	auto up = glm::normalize(glm::cross(_direction, right));
	_direction = glm::rotate(_direction, dt * angle * _turnSpeed, up);
}

void Camera::turnRight(std::uint32_t diff, float angle)
{
	float dt = static_cast<float>(diff) / 1000.0f;
	auto right = glm::normalize(glm::cross(_direction, glm::vec3(0.0, 1.0f, 0.0f)));
	auto up = glm::normalize(glm::cross(_direction, right));
	_direction = glm::rotate(_direction, -(dt * angle * _turnSpeed), up);
}

void Camera::turnUp(std::uint32_t diff, float angle)
{
	float dt = static_cast<float>(diff) / 1000.0f;
	auto right = glm::normalize(glm::cross(_direction, glm::vec3(0.0, 1.0f, 0.0f)));
	_direction = glm::rotate(_direction, dt * angle * _turnSpeed, right);
}

void Camera::turnDown(std::uint32_t diff, float angle)
{
	float dt = static_cast<float>(diff) / 1000.0f;
	auto right = glm::normalize(glm::cross(_direction, glm::vec3(0.0, 1.0f, 0.0f)));
	_direction = glm::rotate(_direction, -(dt * angle * _turnSpeed), right);
}