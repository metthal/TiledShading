#include <chrono>
#include <iostream>

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
	config.debug = true;

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
	window.getScene()->addLight(std::make_shared<Light>(glm::vec3{ 5.0f, 2.0f, 0.0f }, glm::vec3{ 1.0f, 0.0f, 0.0f }, 3.0f));
	window.getScene()->addLight(std::make_shared<Light>(glm::vec3{ -5.0f, 2.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, 3.0f));
	window.getScene()->addLight(std::make_shared<Light>(glm::vec3{ 0.0f, 4.0f, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f }, 30.0f));

	window.gameLoop();
	return 0;
}