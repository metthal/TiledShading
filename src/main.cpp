#include <chrono>
#include <iostream>
#include <random>

#include <SDL.h>

#include "scene/camera.h"
#include "shaders/shader.h"
#include "shaders/shader_program.h"
#include "mesh/mesh.h"
#include "window/window.h"
#include "shaders/deferred_shader_pipeline.h"

const std::uint32_t Width = 1366;
const std::uint32_t Height = 768;

int main(int argc, char** argv)
{
	OpenGLConfig config;
	config.versionMajor = 4;
	config.versionMinor = 3;
	//config.debug = true;

	std::string error;
	Window window("PGP - Tiled Shading", Width, Height);
	if (!window.init(config, error))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", error.c_str(), nullptr);
		return 1;
	}

	if (!window.addPipeline(std::make_shared<DeferredShaderPipeline>(), error))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", error.c_str(), nullptr);
		return 1;
	}

	auto bunnyMesh = Mesh::load("bunny.obj");
	auto floorMesh = Mesh::load("floor.obj");

	auto bunny = std::make_shared<Object>(glm::vec3{ 0.0f, 0.2f, 0.0f }, bunnyMesh);
	bunny->setScale(0.6f);

	auto floor = std::make_shared<Object>(glm::vec3{ 0.0f, 0.0f, 0.0f }, floorMesh);
	floor->setScale(10.0f);

	window.getScene()->getCamera()->setPosition({ 0.0f, 5.0f, 10.0f });
	window.getScene()->getCamera()->lookAt({ 0.0f, 0.0f, 0.0f });
	window.getScene()->getCamera()->setFieldOfView(45.0f);
	window.getScene()->getCamera()->setAspectRatio(Width, Height);
	window.getScene()->addObject(bunny);
	window.getScene()->addObject(floor);

	std::mt19937 rng(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
	std::uniform_real_distribution<float> uniform;
	std::uniform_real_distribution<float>::param_type position(-10.0f, 10.0f);
	std::uniform_real_distribution<float>::param_type color(0.0f, 1.0f);
	std::uniform_real_distribution<float>::param_type velocity(3.0f, 5.0f);
	for (int i = 0; i < 64; ++i)
	{
		float x = uniform(rng, position);
		float z = uniform(rng, position);
		float r = uniform(rng, color);
		float g = uniform(rng, color);
		float b = uniform(rng, color);
		float vx = uniform(rng, velocity);
		float vz = uniform(rng, velocity);

		auto light = std::make_shared<Light>(glm::vec3{ x, 0.5f, z }, glm::vec3{ r, g, b }, 3.0f);
		light->setVelocity({ vx, 0.0f, vz });
		window.getScene()->addLight(light);
	}

	window.gameLoop();
	return 0;
}