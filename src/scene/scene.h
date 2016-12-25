#ifndef TILED_SHADING_SCENE_SCENE_H
#define TILED_SHADING_SCENE_SCENE_H

#include <memory>
#include <vector>

#include "scene/camera.h"
#include "scene/light.h"
#include "scene/object.h"

class Scene
{
public:
	std::size_t getNumberOfLights() const;
	Camera* getCamera();
	const Camera* getCamera() const;
	const std::vector<std::shared_ptr<Light>>& getLights() const;
	bool areLightsMoving() const;

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
	Camera _camera;
	std::vector<std::shared_ptr<Light>> _lights;
	std::vector<std::shared_ptr<Object>> _objects;
	bool _moveLights = true;
};

#endif