#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL.h>

#include "scene/camera.h"
#include "shaders/shader.h"
#include "shaders/shader_program.h"

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

	auto vertShader = Shader::loadVertexShader("vs.vert");
	if (!vertShader)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", vertShader.getError().c_str(), window);
		return 1;
	}

	auto fragShader = Shader::loadFragmentShader("fs.frag");
	if (!fragShader)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", fragShader.getError().c_str(), window);
		return 1;
	}

	auto program = ShaderProgram::link(vertShader, fragShader);
	if (!program)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", program.getError().c_str(), window);
		return 1;
	}
	program.use();

	GLuint vao;
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	std::vector<float> square
	{
		-0.5, -0.5, 0.0,
		0.5, 0.5, 0.0,
		-0.5, 0.5, 0.0,
		-0.5, -0.5, 0.0,
		0.5, -0.5, 0.0,
		0.5, 0.5, 0.0
	};

	GLuint vbo;
	glCreateBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, square.size() * sizeof(float), square.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexArrayAttrib(vao, 0);

	Camera camera({ 0.1f, 0.0f, -0.8f }, { 0.0f, 0.0f, 0.0f });
	auto mvp = camera.getViewTransform();

	auto mvpUniform = glGetUniformLocation(program.getId(), "mvp");
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));

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
				default:
					break;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
    return 0;
}
