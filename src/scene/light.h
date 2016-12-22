#ifndef TILED_SHADING_SCENE_LIGHT_H
#define TILED_SHADING_SCENE_LIGHT_H

#include <glm/glm.hpp>

class Light
{
public:
	Light(const glm::vec3& position, const glm::vec3& intensity);

	const glm::vec3& getPosition() const;
	const glm::vec3& getIntensity() const;

private:
	glm::vec3 _position;
	glm::vec3 _intensity;
};

#endif