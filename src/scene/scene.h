#ifndef TILED_SHADING_SCENE_SCENE_H
#define TILED_SHADING_SCENE_SCENE_H

#include <memory>
#include <vector>

#include "scene/camera.h"
#include "scene/light.h"
#include "scene/object.h"

const std::size_t MaxLights = 1024;

struct LightData
{
	float attenuation;
	float radius;
	float padding1[2];
	glm::vec3 position;
	float padding2[1];
	glm::vec3 intensity;
	float padding3[1];
};

class Scene
{
public:
	Scene();
	~Scene();

	std::size_t getNumberOfLights() const;
	Camera* getCamera();
	const Camera* getCamera() const;
	const std::vector<std::shared_ptr<Light>>& getLights() const;
	bool areLightsMoving() const;
	GLuint getLightsBufferId() const;

	void setMoveLights(bool set);

	void addLight(const std::shared_ptr<Light>& light);
	void addLight(std::shared_ptr<Light>&& light);
	void addObject(const std::shared_ptr<Object>& object);
	void addObject(std::shared_ptr<Object>&& object);

	auto begin() { return _objects.begin(); }
	auto end() { return _objects.end(); }

	auto begin() const { return _objects.begin(); }
	auto end() const { return _objects.end(); }

	void update(std::uint32_t diff);

	void removeLights(std::size_t count);
	void generateLights(std::size_t count);

private:
	void _removeLights();
	void _generateLights();

	Camera _camera;
	std::vector<std::shared_ptr<Light>> _lights;
	std::vector<std::shared_ptr<Object>> _objects;
	bool _moveLights = true;
	GLuint _lightsUbo = 0;

	bool _newMoveLights = true;
	std::size_t _newLightsCount = 0;
};

#endif