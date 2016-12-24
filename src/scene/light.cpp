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
	_attenuation = 1.0f / (_radius * _radius) * 10.0f;
}