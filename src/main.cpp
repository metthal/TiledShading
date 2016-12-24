#include <chrono>
#include <iostream>

#include <SDL.h>

#include "scene/camera.h"
#include "shaders/shader.h"
#include "shaders/shader_program.h"
#include "mesh/mesh.h"
#include "window/window.h"
#include "shaders/deferred_shader_pipeline.h"

int main(int argc, char** argv)
{
	OpenGLConfig config;
	config.versionMajor = 4;
	config.versionMinor = 3;
	config.debug = true;

	std::string error;
	Window window("PGP - Tiled Shading", 800, 600);
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

	auto floor = std::make_shared<Object>(glm::vec3{ 0.0f, -1.0f, 0.0f }, floorMesh);
	floor->setScale(10.0f);

	window.getScene()->setCamera(std::make_shared<Camera>(glm::vec3{ -5.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f }, 60.0f, 800.0f / 600.0f));
	window.getScene()->addObject(std::make_shared<Object>(glm::vec3{ 0.0f, 0.0f, 0.0f }, bunnyMesh));
	window.getScene()->addObject(floor);
	window.getScene()->addLight(std::make_shared<Light>(glm::vec3{ 5.0f, 0.01f, 0.0f }, glm::vec3{ 1.0f, 0.0f, 0.0f }, 5.0f));
	window.getScene()->addLight(std::make_shared<Light>(glm::vec3{ -5.0f, 0.01f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, 5.0f));

	window.gameLoop();
	return 0;
}