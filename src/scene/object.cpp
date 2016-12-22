#include <glm/gtx/transform.hpp>

#include "scene/object.h"

Object::Object(const glm::vec3& position, const std::shared_ptr<Mesh>& mesh) : _position(position), _mesh(mesh)
{
}

const glm::vec3& Object::getPosition() const
{
	return _position;
}

const std::shared_ptr<Mesh>& Object::getMesh() const
{
	return _mesh;
}

glm::mat4 Object::getTransform() const
{
	return glm::translate(_position);
}