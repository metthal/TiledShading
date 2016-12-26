#include "scene/light.h"

Light::Light(const glm::vec3& position, const glm::vec3& intensity, float radius) : _position(position), _intensity(intensity)
{
	setRadius(radius);
}

const glm::vec3& Light::getPosition() const
{
	return _position;
}

const glm::vec3& Light::getIntensity() const
{
	return _intensity;
}

float Light::getAttenuation() const
{
	return _attenuation;
}

float Light::getRadius() const
{
	return _radius;
}

const glm::vec3& Light::getVelocity() const
{
	return _velocity;
}

void Light::setPosition(const glm::vec3& position)
{
	_position = position;
}

void Light::setIntensity(const glm::vec3& intensity)
{
	_intensity = intensity;
}

void Light::setRadius(float radius)
{
	_radius = radius;

	float k = 0.01f;
	_attenuation = (1.0f - k) / (k * _radius * _radius);
}

void Light::setVelocity(const glm::vec3& velocity)
{
	_velocity = velocity;
}

void Light::update(std::uint32_t diff)
{
	float dt = static_cast<float>(diff) / 1000.0f;
	_position += dt * _velocity;
}