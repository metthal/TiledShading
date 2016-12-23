#include <glm/gtx/transform.hpp>

#include "scene/object.h"

Object::Object(const glm::vec3& position, const std::shared_ptr<Mesh>& mesh) : _position(position), _mesh(mesh), _scale(1.0f), _color(1.0f, 1.0f, 1.0f)
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

float Object::getScale() const
{
	return _scale;
}

const glm::vec3& Object::getColor() const
{
	return _color;
}

void Object::setScale(float scale)
{
	_scale = scale;
}

glm::mat4 Object::getTransform() const
{
	return glm::translate(_position) * glm::scale(glm::vec3{ _scale, _scale, _scale });
}