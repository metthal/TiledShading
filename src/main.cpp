#include <iostream>

#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL.h>

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

	std::cout << "OpenGL Information\n"
		<< " Renderer = " << glGetString(GL_RENDERER) << '\n'
		<< " Version = " << glGetString(GL_VERSION) << '\n'
		<< " Shading Language Version = " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	glClearColor(0.13f, 0.0f, 0.31f, 1.0f);

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

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
    return 0;
}