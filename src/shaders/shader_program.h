#ifndef TILED_SHADING_SHADER_PROGRAM_H
#define TILED_SHADING_SHADER_PROGRAM_H

#include <vector>

#include <GL/glew.h>

#include "shaders/shader.h"

class ShaderProgram
{
public:
	template <typename... Shaders>
	static ShaderProgram link(const Shaders&... shaders)
	{
		auto id = glCreateProgram();
		return linkImpl(id, shaders...);
	}

	operator bool() const;

	GLuint getId() const;
	const std::string& getError() const;

	void use();

private:
	ShaderProgram(const std::string& error);
	ShaderProgram(GLuint id);

	static ShaderProgram linkImpl(GLuint id, const Shader& shader)
	{
		glAttachShader(id, shader.getId());
		glLinkProgram(id);

		GLint linkStatus;
		glGetProgramiv(id, GL_LINK_STATUS, &linkStatus);
		if (linkStatus == GL_FALSE)
		{
			GLint linkErrorLength;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &linkErrorLength);

			GLint ret;
			std::vector<GLchar> errorData(linkErrorLength);
			glGetProgramInfoLog(id, linkErrorLength, &ret, errorData.data());

			return { std::string(errorData.begin(), errorData.end()) };
		}

		return { id };
	}

	template <typename... Shaders>
	static ShaderProgram linkImpl(GLuint id, const Shader& shader, const Shaders&... shaders)
	{
		glAttachShader(id, shader.getId());
		return linkImpl(id, shaders...);
	}

	GLuint _id;
	std::string _error;
};

#endif