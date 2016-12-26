#include <algorithm>
#include <chrono>
#include <random>

#include "scene/scene.h"

Scene::Scene() : _camera(), _lights(), _objects(), _moveLights(true), _lightsUbo(0)
{
	glGenBuffers(1, &_lightsUbo);
	glBindBuffer(GL_UNIFORM_BUFFER, _lightsUbo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(LightData) * MaxLights, nullptr, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Scene::~Scene()
{
	glDeleteBuffers(1, &_lightsUbo);
}

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

GLuint Scene::getLightsBufferId() const
{
	return _lightsUbo;
}

void Scene::setMoveLights(bool set)
{
	_newMoveLights = set;
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
	_moveLights = _newMoveLights;
	if (getNumberOfLights() < _newLightsCount)
		_generateLights();
	else if (getNumberOfLights() > _newLightsCount)
		_removeLights();

	glBindBuffer(GL_UNIFORM_BUFFER, _lightsUbo);
	auto lightsBuffer = static_cast<LightData*>(glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY));
	std::size_t i = 0;
	if (_moveLights)
	{
		for (const auto& light : _lights)
		{
			light->update(diff);

			auto velocity = light->getVelocity();
			auto position = light->getPosition();

			if (std::fabs(light->getPosition().x) > 10.0f)
			{
				velocity = { -velocity.x, velocity.y, velocity.z };
				position = { light->getPosition().x > 0.0f ? 9.98f : -9.98f , position.y, position.z };
			}

			if (std::fabs(light->getPosition().z) > 10.0f)
			{
				velocity = { velocity.x, velocity.y, -velocity.z };
				position = { position.x, position.y,  light->getPosition().z > 0.0f ? 9.98f : -9.98f};
			}

			light->setVelocity(velocity);
			light->setPosition(position);

			lightsBuffer[i].position = light->getPosition();
			lightsBuffer[i].intensity = light->getIntensity();
			lightsBuffer[i].attenuation = light->getAttenuation();
			lightsBuffer[i].radius = light->getRadius();
			i++;
		}
	}
	else
	{
		for (const auto& light : _lights)
		{
			lightsBuffer[i].position = light->getPosition();
			lightsBuffer[i].intensity = light->getIntensity();
			lightsBuffer[i].attenuation = light->getAttenuation();
			lightsBuffer[i].radius = light->getRadius();
			i++;
		}
	}
	glUnmapBuffer(GL_UNIFORM_BUFFER);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Scene::removeLights(std::size_t count)
{
	_newLightsCount = getNumberOfLights() - count;
}

void Scene::generateLights(std::size_t count)
{
	_newLightsCount = getNumberOfLights() + count;
}

void Scene::_removeLights()
{
	_lights.resize(_newLightsCount);
}

void Scene::_generateLights()
{
	static std::mt19937 rng(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
	static std::uniform_real_distribution<float> uniform;
	static std::uniform_real_distribution<float>::param_type position(-9.9f, 9.9f);
	static std::uniform_real_distribution<float>::param_type color(0.0f, 1.0f);
	static std::uniform_real_distribution<float>::param_type radius(4.0f, 6.0f);
	static std::uniform_real_distribution<float>::param_type velocity(1.0f, 5.0f);
	static std::discrete_distribution<int> velocityDirection({ 1, 0, 1 });

	glBindBuffer(GL_UNIFORM_BUFFER, _lightsUbo);
	auto lightsBuffer = static_cast<LightData*>(glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY));
	for (std::size_t i = getNumberOfLights(); i < _newLightsCount; ++i)
	{
		float x = uniform(rng, position);
		float z = uniform(rng, position);
		float r = uniform(rng, color);
		float g = uniform(rng, color);
		float b = uniform(rng, color);
		float vx = static_cast<float>(velocityDirection(rng) - 1) * uniform(rng, velocity);
		float vz = static_cast<float>(velocityDirection(rng) - 1) * uniform(rng, velocity);
		float rad = uniform(rng, radius);

		auto light = std::make_shared<Light>(glm::vec3{ x, 0.5f, z }, glm::vec3{ r, g, b }, rad);
		light->setVelocity({ vx, 0.0f, vz });
		addLight(light);

		lightsBuffer[i].position = light->getPosition();
		lightsBuffer[i].intensity = light->getIntensity();
		lightsBuffer[i].attenuation = light->getAttenuation();
		lightsBuffer[i].radius = light->getRadius();
	}
	glUnmapBuffer(GL_UNIFORM_BUFFER);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}