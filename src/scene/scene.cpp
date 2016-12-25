#include "scene/scene.h"

std::size_t Scene::getNumberOfLights() const
{
	return _lights.size();
}

Camera* Scene::getCamera()
{
	return &_camera;
}

const Camera* Scene::getCamera() const
{
	return &_camera;
}

const std::vector<std::shared_ptr<Light>>& Scene::getLights() const
{
	return _lights;
}

bool Scene::areLightsMoving() const
{
	return _moveLights;
}

void Scene::setMoveLights(bool set)
{
	_moveLights = set;
}

void Scene::addLight(const std::shared_ptr<Light>& light)
{
	_lights.push_back(light);
}

void Scene::addLight(std::shared_ptr<Light>&& light)
{
	_lights.push_back(std::move(light));
}

void Scene::addObject(const std::shared_ptr<Object>& object)
{
	_objects.push_back(object);
}

void Scene::addObject(std::shared_ptr<Object>&& object)
{
	_objects.push_back(std::move(object));
}

void Scene::update(std::uint32_t diff)
{
	if (_moveLights)
	{
		for (const auto& light : _lights)
		{
			light->update(diff);

			auto velocity = light->getVelocity();
			if (std::fabs(light->getPosition().x) > 10.0f)
				velocity = { -velocity.x, velocity.y, velocity.z };

			if (std::fabs(light->getPosition().z) > 10.0f)
				velocity = { velocity.x, velocity.y, -velocity.z };

			light->setVelocity(velocity);
		}
	}
}