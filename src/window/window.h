#ifndef TILED_SHADING_WINDOW_WINDOW_H
#define TILED_SHADING_WINDOW_WINDOW_H

#include <memory>
#include <string>

#include <glm/glm.hpp>
#include <imgui.h>
#include <SDL.h>

#include "scene/scene.h"
#include "shaders/pipeline.h"
#include "shaders/imgui_pipeline.h"

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

	SDL_Window* getImpl() const;
	const glm::ivec2& getDimensions() const;
	Scene* getScene();
	const Scene* getScene() const;
	bool hasVsync() const;

	void setVsync(bool set);

	Pipeline* getActivePipeline() const;
	std::size_t getActivePipelineIndex() const;
	bool addPipeline(const std::shared_ptr<Pipeline>& pipeline, std::string& error);
	void switchPipeline(std::size_t index);

private:
	void handleEvent(const SDL_Event& event, std::uint32_t diff);
	void handleKeydown(const SDL_Event& event, std::uint32_t diff);
	void handleMouseMove(const SDL_Event& event, std::uint32_t diff);

	std::string _title;
	glm::ivec2 _dimensions;
	std::unique_ptr<Scene> _scene;
	std::vector<std::shared_ptr<Pipeline>> _pipelines;
	ImguiPipeline _imguiPipeline;

	SDL_Window* _impl;
	SDL_GLContext _glContext;
	std::size_t _currentPipelineIndex;
	Pipeline* _currentPipeline;
};

#endif