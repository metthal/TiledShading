#include "shaders/shader_program.h"

ShaderProgram::~ShaderProgram()
{
	if (_id != ~0)
		glDeleteProgram(_id);
}

ShaderProgram::ShaderProgram(GLuint id, std::vector<std::unique_ptr<Shader>>&& shaders) : _id(id), _numAttributes(0), _shaders(std::move(shaders)), _error()
{
	init();
}

ShaderProgram::ShaderProgram(const std::string& error) : _id(~0), _numAttributes(0), _shaders(), _error(error)
{
}

ShaderProgram::operator bool() const
{
	return _error.empty();
}

GLuint ShaderProgram::getId() const
{
	return _id;
}

GLint ShaderProgram::getNumberOfAttributes() const
{
	return _numAttributes;
}

const std::string& ShaderProgram::getError() const
{
	return _error;
}

void ShaderProgram::use()
{
	glUseProgram(_id);
}

void ShaderProgram::init()
{
	glGetProgramiv(_id, GL_ACTIVE_ATTRIBUTES, &_numAttributes);
}