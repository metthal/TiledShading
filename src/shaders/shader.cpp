#include <fstream>
#include <vector>

#include "shaders/shader.h"
#include "utils/utils.h"

Shader::Shader(std::uint32_t type, GLuint id) : _type(type), _id(id)
{
}

Shader::~Shader()
{
	glDeleteShader(_id);
}

std::shared_ptr<Shader> Shader::loadString(GLuint type, const std::string& shaderSource, std::string& error)
{
	auto contentSources = shaderSource.c_str();
	auto contentLengths = static_cast<GLint>(shaderSource.length());

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

		error = std::string(errorData.begin(), errorData.end());
		return nullptr;
	}

	return std::make_shared<Shader>(type, id);
}

std::shared_ptr<Shader> Shader::loadFile(const std::string& shaderName, std::string& error)
{
	auto fileName = "shaders/" + shaderName;
	GLuint type;
	if (endsWith(fileName, ".vert"))
		type = GL_VERTEX_SHADER;
	else if (endsWith(fileName, ".frag"))
		type = GL_FRAGMENT_SHADER;
	else
	{
		error = "Unable to determine type of shader from its extension";
		return nullptr;
	}

	std::ifstream file(fileName);
	if (!file.is_open())
	{
		error = "Unable to open shader file '" + fileName + "'";
		return nullptr;
	}

	std::string content, line;
	while (std::getline(file, line))
		content += line + '\n';

	return Shader::loadString(type, content, error);
}

std::uint32_t Shader::getType() const
{
	return _type;
}

GLuint Shader::getId() const
{
	return _id;
}
