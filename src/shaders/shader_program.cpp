#include "shaders/shader_program.h"

ShaderProgram::ShaderProgram(const std::string& error) : _id(~0), _error(error)
{
}

ShaderProgram::ShaderProgram(GLuint id) : _id(id), _error()
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

const std::string& ShaderProgram::getError() const
{
	return _error;
}

void ShaderProgram::use()
{
	glUseProgram(_id);
}