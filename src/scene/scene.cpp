#include "scene/scene.h"

std::size_t Scene::getNumberOfLights() const
{
	return _lights.size();
}

const std::shared_ptr<Camera>& Scene::getCamera() const
{
	return _camera;
}

const std::vector<std::shared_ptr<Light>>& Scene::getLights() const
{
	return _lights;
}

void Scene::setCamera(const std::shared_ptr<Camera>& camera)
{
	_camera = camera;
}

void Scene::setCamera(std::shared_ptr<Camera>&& camera)
{
	_camera = std::move(camera);
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