#ifndef TILED_SHADING_SCENE_OBJECT_H
#define TILED_SHADING_SCENE_OBJECT_H

#include <memory>

#include <glm/glm.hpp>

#include "mesh/mesh.h"

class Object
{
public:
	Object(const glm::vec3& position, const std::shared_ptr<Mesh>& mesh);

	const glm::vec3& getPosition() const;
	const std::shared_ptr<Mesh>& getMesh() const;
	float getScale() const;
	const glm::vec3& getColor() const;

	void setPosition(const glm::vec3& position);
	void setScale(float scale);
	void setColor(const glm::vec3& color);

	glm::mat4 getTransform() const;

private:
	glm::vec3 _position;
	std::shared_ptr<Mesh> _mesh;
	float _scale;
	glm::vec3 _color;
};

#endif