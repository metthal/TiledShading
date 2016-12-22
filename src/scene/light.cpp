#include "scene/light.h"

Light::Light(const glm::vec3& position, const glm::vec3& intensity) : _position(position), _intensity(intensity)
{
}

const glm::vec3& Light::getPosition() const
{
	return _position;
}

const glm::vec3& Light::getIntensity() const
{
	return _intensity;
}