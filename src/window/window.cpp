#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include "window/window.h"

namespace {

void __stdcall openglDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::cerr << message << std::endl;
}

}

Window::Window(const std::string& title, const glm::ivec2& dimensions)
	: _title(title), _dimensions(dimensions), _scene(), _pipelines(), _imguiPipeline(), _impl(nullptr), _glContext(nullptr), _currentPipeline(nullptr)
{
}

Window::Window(const std::string& title, std::size_t width, std::size_t height)
	: Window(title, { width, height })
{
}

Window::~Window()
{
	if (_glContext != nullptr)
		SDL_GL_DeleteContext(_glContext);

	if (_impl != nullptr)
		SDL_DestroyWindow(_impl);
}

bool Window::init(const OpenGLConfig& config, std::string& error)
{
	if (_impl != nullptr)
		return false;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, config.versionMajor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, config.versionMinor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	if (config.debug)
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	_impl = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _dimensions.x, _dimensions.y, SDL_WINDOW_OPENGL);
	if (_impl == nullptr)
	{
		error = "Unable to create window";
		return false;
	}

	_glContext = SDL_GL_CreateContext(_impl);
	if (_glContext == nullptr)
	{
		error = "Unable to create OpenGL context";
		return false;
	}

	if (glewInit() != GLEW_OK)
	{
		error = "Unable to initialize GLEW";
		return false;
	}

	if (config.debug)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(openglDebugCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
	}

	SDL_GL_SetSwapInterval(0);
	glClearColor(config.clearColor.r, config.clearColor.g, config.clearColor.b, 1.0f);

	if (!_imguiPipeline.init(this, error))
		return false;

	return true;
}

void Window::gameLoop()
{
	SDL_Event event;
	bool running = true;

	std::uint32_t lastDiff = SDL_GetTicks();
	while (running)
	{
		std::uint32_t now = SDL_GetTicks();
		std::uint32_t diff = now - lastDiff;
		lastDiff = now;

		while (SDL_PollEvent(&event) > 0)
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
				break;
			}

			handleEvent(event, diff);
			_imguiPipeline.handleEvent(event);
		}

		_scene.update(diff);

		_currentPipeline->run(this, diff);
		_imguiPipeline.run(this, diff);

		SDL_GL_SwapWindow(_impl);
	}
}

SDL_Window* Window::getImpl() const
{
	return _impl;
}

const glm::ivec2& Window::getDimensions() const
{
	return _dimensions;
}

Scene* Window::getScene()
{
	return &_scene;
}

const Scene* Window::getScene() const
{
	return &_scene;
}

bool Window::addPipeline(const std::shared_ptr<Pipeline>& pipeline, std::string& error)
{
	if (!pipeline->init(this, error))
		return false;

	if (_currentPipeline == nullptr)
		_currentPipeline = pipeline.get();
	_pipelines.push_back(pipeline);
	return true;
}

void Window::switchPipeline(std::size_t index)
{
	if (index >= _pipelines.size())
		return;

	_currentPipeline = _pipelines[index].get();
}

void Window::handleEvent(const SDL_Event& event, std::uint32_t diff)
{
	switch (event.type)
	{
		case SDL_KEYDOWN:
			handleKeydown(event, diff);
			break;
		case SDL_MOUSEMOTION:
			handleMouseMove(event, diff);
			break;
		default:
			return;
	}
}

void Window::handleKeydown(const SDL_Event& event, std::uint32_t diff)
{
	switch (event.key.keysym.sym)
	{
		case SDLK_w:
			_scene.getCamera()->moveForwards(diff);
			break;
		case SDLK_s:
			_scene.getCamera()->moveBackwards(diff);
			break;
		case SDLK_a:
			_scene.getCamera()->strafeLeft(diff);
			break;
		case SDLK_d:
			_scene.getCamera()->strafeRight(diff);
			break;
		default:
			return;
	}
}

void Window::handleMouseMove(const SDL_Event& event, std::uint32_t diff)
{
	if (event.motion.state & SDL_BUTTON_RMASK)
	{
		_scene.getCamera()->turnUp(diff, event.motion.yrel * static_cast<float>(M_PI) / 180.0f);
		_scene.getCamera()->turnRight(diff, event.motion.xrel * static_cast<float>(M_PI) / 180.0f);
	}
}