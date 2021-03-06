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
	const glm::vec3& getVelocity() const;

	void setPosition(const glm::vec3& position);
	void setIntensity(const glm::vec3& intensity);
	void setRadius(float radius);
	void setVelocity(const glm::vec3& velocity);

	void update(std::uint32_t diff);

private:
	glm::vec3 _position;
	glm::vec3 _intensity;
	float _radius;
	glm::vec3 _velocity;

	float _attenuation;
};

#endif