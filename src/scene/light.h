#ifndef TILED_SHADING_SCENE_LIGHT_H
#define TILED_SHADING_SCENE_LIGHT_H

#include <glm/glm.hpp>

class Light
{
public:
	Light(const glm::vec3& position, const glm::vec3& intensity, float radius);

	const glm::vec3& getPosition() const;
	const glm::vec3& getIntensity() const;

	float getAttenuation() const;
	float getRadius() const;

	void setRadius(float radius);

private:
	glm::vec3 _position;
	glm::vec3 _intensity;
	float _radius;

	float _attenuation;
};

#endif