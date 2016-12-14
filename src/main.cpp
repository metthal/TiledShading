#include <iostream>

#include <assimp/scene.h>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL.h>

#include "scene/camera.h"
#include "shaders/shader.h"
#include "shaders/shader_program.h"
#include "mesh/mesh.h"

int main(int argc, char** argv)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

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

	std::cout << "OpenGL Information\n"
		<< " Renderer = " << glGetString(GL_RENDERER) << '\n'
		<< " Version = " << glGetString(GL_VERSION) << '\n'
		<< " Shading Language Version = " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	glClearColor(0.13f, 0.0f, 0.31f, 1.0f);

	auto vertShader = Shader::loadVertexShader("shaders/vs.vert");
	if (!vertShader)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", vertShader.getError().c_str(), window);
		return 1;
	}

	auto fragShader = Shader::loadFragmentShader("shaders/fs.frag");
	if (!fragShader)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", fragShader.getError().c_str(), window);
		return 1;
	}

	Camera camera({ 5.0f, 10.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });

	auto program = ShaderProgram::link(vertShader, fragShader);
	if (!program)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", program.getError().c_str(), window);
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	program.use();
	auto mvpUniform = glGetUniformLocation(program.getId(), "mvp");
	auto bunny = Mesh::loadFromFile("models/bunny.obj");

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
						camera.turnRight(event.motion.xrel * M_PI / 180.0f);
						camera.turnDown(event.motion.yrel * M_PI / 180.0f);
					}
					break;
				}
				default:
					break;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program.use();
		glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(camera.getViewTransform()));
		bunny.render();

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
    return 0;
}
