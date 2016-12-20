#include <chrono>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL.h>

#include "scene/camera.h"
#include "shaders/shader.h"
#include "shaders/shader_program.h"
#include "mesh/mesh.h"
#include "deferred_shading/g_buffer.h"

bool loadShadersImpl(std::vector<std::unique_ptr<ShaderProgram>>& result)
{
	return true;
}

template <typename... Files>
bool loadShadersImpl(std::vector<std::unique_ptr<ShaderProgram>>& result, const std::string& vertexShaderFile, const std::string& fragmentShaderFile, const Files&... files)
{
	auto vertexShader = Shader::loadVertexShader(vertexShaderFile);
	if (!vertexShader)
	{
		auto errorStr = vertexShaderFile + ": " + vertexShader->getError();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", errorStr.c_str(), nullptr);
		result.clear();
		return false;
	}

	auto fragmentShader = Shader::loadFragmentShader(fragmentShaderFile);
	if (!fragmentShader)
	{
		auto errorStr = fragmentShaderFile + ": " + fragmentShader->getError();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", errorStr.c_str(), nullptr);
		result.clear();
		return false;
	}

	auto program = ShaderProgram::link(std::move(vertexShader), std::move(fragmentShader));
	if (!program)
	{
		auto errorStr = vertexShaderFile + "/" + fragmentShaderFile + ": " + program->getError();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", errorStr.c_str(), nullptr);
		result.clear();
		return false;
	}

	result.push_back(std::move(program));
	return loadShadersImpl(result, files...);
}

template <typename... Files>
std::vector<std::unique_ptr<ShaderProgram>> loadShaders(const Files&... files)
{
	std::vector<std::unique_ptr<ShaderProgram>> result;
	loadShadersImpl(result, files...);
	return result;
}

//static void __stdcall openglCallbackFunction(
//	GLenum source,
//	GLenum type,
//	GLuint id,
//	GLenum severity,
//	GLsizei length,
//	const GLchar* message,
//	const void* userParam
//)
//{
//	std::cerr << message << std::endl;
//}

int main(int argc, char** argv)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	auto window = SDL_CreateWindow("PGP - Tiled Shading", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);
	if (!window)
	{
		std::cerr << "Unable to create SDL window\n";
		return 1;
	}

	auto context = SDL_GL_CreateContext(window);
	if (!context)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Unable to create OpenGL context", window);
		return 1;
	}

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW\n";
		return 1;
	}

	// Enable the debug callback
	//glEnable(GL_DEBUG_OUTPUT);
	//glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	//glDebugMessageCallback(openglCallbackFunction, nullptr);
	//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);

	std::cout << "OpenGL Information\n"
		<< " Renderer = " << glGetString(GL_RENDERER) << '\n'
		<< " Version = " << glGetString(GL_VERSION) << '\n'
		<< " Shading Language Version = " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	glClearColor(0.13f, 0.0f, 0.31f, 1.0f);

	Camera camera({ 5.0f, 10.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	glm::vec3 lightPos = { 5.0f, 10.0f, 0.0f };

	glEnable(GL_DEPTH_TEST);

	auto bunny = Mesh::loadFromFile("models/bunny.obj");
	auto model = glm::translate(glm::vec3{ 0.0f, 0.0f, 0.0f });

	auto programs = loadShaders(
		"shaders/deferred_shader.vert", "shaders/deferred_shader.frag",
		"shaders/deferred_shader_light_pass.vert", "shaders/deferred_shader_light_pass.frag"
	);
	if (programs.empty())
		return 1;

	auto& deferredShaderProgram = programs[0];
	auto& deferredShaderLightPassProgram = programs[1];

	deferredShaderProgram->use();
	auto modelUniform_pass1 = glGetUniformLocation(deferredShaderProgram->getId(), "model");
	auto viewProjectUniform_pass1 = glGetUniformLocation(deferredShaderProgram->getId(), "viewProject");

	deferredShaderLightPassProgram->use();
	auto modelUniform_pass2 = glGetUniformLocation(deferredShaderLightPassProgram->getId(), "model");
	auto viewProjectUniform_pass2 = glGetUniformLocation(deferredShaderLightPassProgram->getId(), "viewProject");
	auto lightPosUniform = glGetUniformLocation(deferredShaderLightPassProgram->getId(), "lightPos");
	auto gbufferPosUniform = glGetUniformLocation(deferredShaderLightPassProgram->getId(), "gbufferPos");
	auto gbufferNormalUniform = glGetUniformLocation(deferredShaderLightPassProgram->getId(), "gbufferNormal");
	auto gbufferAlbedoUniform = glGetUniformLocation(deferredShaderLightPassProgram->getId(), "gbufferAlbedo");

	GBuffer gbuffer;
	if (!gbuffer.init())
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to create framebuffer for deferred shader.", window);
		return 1;
	}

	bool running = true;
	while (running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event) > 0)
		{
			switch (event.type)
			{
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
				{
					switch (event.key.keysym.sym)
					{
						case SDLK_w:
							camera.moveForwards(0.1f);
							break;
						case SDLK_s:
							camera.moveBackwards(0.1f);
							break;
						case SDLK_a:
							camera.strafeLeft(0.1f);
							break;
						case SDLK_d:
							camera.strafeRight(0.1f);
							break;
					}
					break;
				}
				case SDL_MOUSEMOTION:
				{
					if (event.motion.state & SDL_BUTTON_RMASK)
					{
						camera.turnRight(event.motion.xrel * static_cast<float>(M_PI) / 180.0f);
						camera.turnDown(event.motion.yrel * static_cast<float>(M_PI) / 180.0f);
					}
					break;
				}
				default:
					break;
			}
		}

		gbuffer.useFramebuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		deferredShaderProgram->use();
		glUniformMatrix4fv(modelUniform_pass1, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewProjectUniform_pass1, 1, GL_FALSE, glm::value_ptr(camera.getViewTransform()));
		bunny.render();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		deferredShaderLightPassProgram->use();
		gbuffer.useTextures();
		glUniformMatrix4fv(modelUniform_pass2, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewProjectUniform_pass2, 1, GL_FALSE, glm::value_ptr(camera.getViewTransform()));
		glUniform1i(gbufferPosUniform, 0);
		glUniform1i(gbufferNormalUniform, 1);
		glUniform1i(gbufferAlbedoUniform, 2);
		glUniform3fv(lightPosUniform, 1, glm::value_ptr(lightPos));
		bunny.render();

		//glBindFramebuffer(GL_READ_FRAMEBUFFER, gbuffer.getId());
		//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		//glViewport(0, 0, 800, 600);
		//glReadBuffer(GL_COLOR_ATTACHMENT0);
		//glBlitFramebuffer(
		//	0, 0, 800, 600,
		//	0, 300, 400, 600,
		//	GL_COLOR_BUFFER_BIT,
		//	GL_LINEAR);
		//glReadBuffer(GL_COLOR_ATTACHMENT1);
		//glBlitFramebuffer(
		//	0, 0, 800, 600,
		//	400, 300, 800, 600,
		//	GL_COLOR_BUFFER_BIT,
		//	GL_LINEAR);
		//glReadBuffer(GL_COLOR_ATTACHMENT2);
		//glBlitFramebuffer(
		//	0, 0, 800, 600,
		//	0, 0, 400, 300,
		//	GL_COLOR_BUFFER_BIT,
		//	GL_LINEAR);

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
    return 0;
}
