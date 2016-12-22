#ifndef TILED_SHADING_WINDOW_WINDOW_H
#define TILED_SHADING_WINDOW_WINDOW_H

#include <memory>
#include <string>

#include <SDL.h>
#include <glm/glm.hpp>

#include "scene/scene.h"
#include "shaders/pipeline.h"

struct OpenGLConfig
{
	std::uint8_t versionMajor = 4;
	std::uint8_t versionMinor = 3;
	glm::vec3 clearColor = { 0.0f, 0.0f, 0.0f };
	bool debug = false;
};

class Window
{
public:
	using OpenGl = std::tuple<std::uint8_t, std::uint8_t>;

	Window(const std::string& title, const glm::ivec2& dimensions);
	Window(const std::string& title, std::size_t width, std::size_t height);
	~Window();

	bool init(const OpenGLConfig& config, std::string& error);
	void gameLoop();

	const glm::ivec2& getDimensions() const;
	Scene* getScene();

	bool addPipeline(const std::shared_ptr<Pipeline>& pipeline, std::string& error);
	void switchPipeline(std::size_t index);

private:
	void handleEvent(const SDL_Event& event);
	void handleKeydown(const SDL_Event& event);
	void handleMouseMove(const SDL_Event& event);

	std::string _title;
	glm::ivec2 _dimensions;
	Scene _scene;
	std::vector<std::shared_ptr<Pipeline>> _pipelines;

	SDL_Window* _impl;
	SDL_GLContext _glContext;
	Pipeline* _currentPipeline;
};

#endif