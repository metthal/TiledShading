#ifndef TILED_SHADING_SHADER_PROGRAM_H
#define TILED_SHADING_SHADER_PROGRAM_H

#include <memory>
#include <vector>

#include <GL/glew.h>

#include "shaders/shader.h"

class ShaderProgram
{
public:
	ShaderProgram(GLuint id, std::vector<std::unique_ptr<Shader>>&& shaders);
	ShaderProgram(const std::string& error);
	~ShaderProgram();

	template <typename... Shaders>
	static std::unique_ptr<ShaderProgram> link(Shaders&&... shaders)
	{
		auto id = glCreateProgram();
		std::vector<std::unique_ptr<Shader>> shaderPtrs;
		return linkImpl(id, std::move(shaderPtrs), std::forward<Shaders>(shaders)...);
	}

	operator bool() const;

	GLuint getId() const;
	GLint getNumberOfAttributes() const;
	const std::string& getError() const;

	void use();

private:
	static std::unique_ptr<ShaderProgram> linkImpl(GLuint id, std::vector<std::unique_ptr<Shader>>&& shaderPtrs)
	{
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

			return std::make_unique<ShaderProgram>(std::string(errorData.begin(), errorData.end()));
		}

		return std::make_unique<ShaderProgram>(id, std::move(shaderPtrs));
	}

	template <typename... Shaders>
	static std::unique_ptr<ShaderProgram> linkImpl(GLuint id, std::vector<std::unique_ptr<Shader>>&& shaderPtrs, std::unique_ptr<Shader>&& shader, Shaders&&... shaders)
	{
		glAttachShader(id, shader->getId());
		shaderPtrs.push_back(std::move(shader));
		return linkImpl(id, std::move(shaderPtrs), std::forward<Shaders>(shaders)...);
	}

	void init();

	GLuint _id;
	GLint _numAttributes;
	std::vector<std::unique_ptr<Shader>> _shaders;
	std::string _error;
};

#endif