#include <fstream>
#include <vector>

#include "shaders/shader.h"

Shader::~Shader()
{
	if (_id != ~0)
		glDeleteShader(_id);
}

Shader::Shader(const std::string& error) : _type(~0), _id(~0), _error(error)
{
}

Shader::Shader(std::uint32_t type, GLuint id) : _type(type), _id(id), _error()
{
}

std::unique_ptr<Shader> Shader::loadVertexShader(const std::string& fileName)
{
	return loadShader(GL_VERTEX_SHADER, fileName);
}

std::unique_ptr<Shader> Shader::loadFragmentShader(const std::string& fileName)
{
	return loadShader(GL_FRAGMENT_SHADER, fileName);
}

Shader::operator bool() const
{
	return _error.empty();
}

std::uint32_t Shader::getType() const
{
	return _type;
}

GLuint Shader::getId() const
{
	return _id;
}

const std::string& Shader::getError() const
{
	return _error;
}

std::unique_ptr<Shader> Shader::loadShader(std::uint32_t type, const std::string& fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
		return std::make_unique<Shader>("Unable to open shader file '" + fileName + "'!");

	std::string content, line;
	while (std::getline(file, line))
		content += line + '\n';

	auto contentSources = content.c_str();
	GLint contentLengths = static_cast<GLint>(content.length());

	auto id = glCreateShader(type);
	glShaderSource(id, 1, &contentSources, &contentLengths);
	glCompileShader(id);

	GLint compileStatus;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE)
	{
		GLint compileErrorLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &compileErrorLength);

		GLint ret;
		std::vector<GLchar> errorData(compileErrorLength);
		glGetShaderInfoLog(id, compileErrorLength, &ret, errorData.data());

		return std::make_unique<Shader>(std::string(errorData.begin(), errorData.end()));
	}

	return std::make_unique<Shader>(type, id);
}