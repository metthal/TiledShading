#include <glm/gtc/type_ptr.hpp>

#include "shaders/shader_program.h"
#include "utils/utils.h"

ShaderProgram::ShaderProgram(GLuint id, std::vector<std::shared_ptr<Shader>>&& shaders) : _id(id), _numAttributes(0), _numUniforms(0), _shaders(std::move(shaders))
{
	glGetProgramiv(_id, GL_ACTIVE_ATTRIBUTES, &_numAttributes);
	glGetProgramiv(_id, GL_ACTIVE_UNIFORMS, &_numUniforms);

	GLint maxAttributeNameLength, maxUniformNameLength;
	glGetProgramiv(_id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeNameLength);
	glGetProgramiv(_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);

	for (int i = 0; i < _numAttributes; ++i)
	{
		GLsizei attributeNameLength;
		GLint attributeSize;
		GLenum attributeType;
		std::vector<char> attributeNameBuffer(maxAttributeNameLength);
		glGetActiveAttrib(_id, i, maxAttributeNameLength, &attributeNameLength, &attributeSize, &attributeType, attributeNameBuffer.data());

		auto attributeName = std::string(attributeNameBuffer.begin(), attributeNameBuffer.begin() + attributeNameLength);
		_attributes.emplace(attributeName, AttributeInfo{ glGetAttribLocation(_id, attributeName.c_str()) });
	}

	for (int i = 0; i < _numUniforms; ++i)
	{
		GLsizei uniformNameLength;
		GLint uniformSize;
		GLenum uniformType;
		std::vector<char> uniformNameBuffer(maxUniformNameLength);
		glGetActiveUniform(_id, i, maxUniformNameLength, &uniformNameLength, &uniformSize, &uniformType, uniformNameBuffer.data());

		auto uniformName = std::string(uniformNameBuffer.begin(), uniformNameBuffer.begin() + uniformNameLength);

		bool array = false;
		if (endsWith(uniformName, "[0]"))
		{
			uniformName = uniformName.substr(0, uniformName.find('['));
			array = true;
		}

		_uniforms.emplace(uniformName, UniformInfo{ glGetUniformLocation(_id, uniformName.c_str()), array });
	}
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(_id);
}

void ShaderProgram::activate()
{
	glUseProgram(_id);
}

void ShaderProgram::deactivate()
{
	glUseProgram(0);
}

GLuint ShaderProgram::getId() const
{
	return _id;
}

GLint ShaderProgram::getNumberOfAttributes() const
{
	return _numAttributes;
}

GLint ShaderProgram::getNumberOfUniforms() const
{
	return _numUniforms;
}

GLint ShaderProgram::getAttributeId(const std::string& name) const
{
	auto itr = _attributes.find(name);
	if (itr == _attributes.end())
		return -1;

	return itr->second.id;
}

GLint ShaderProgram::getUniformId(const std::string& name) const
{
	auto itr = _uniforms.find(name);
	if (itr == _uniforms.end())
		return -1;

	return itr->second.id;
}

void ShaderProgram::setUniform(const std::string& name, GLint value)
{
	auto id = getUniformId(name);
	if (id == -1)
		return;

	glUniform1i(id, value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec3& v)
{
	auto id = getUniformId(name);
	if (id == -1)
		return;

	glUniform3fv(id, 1, glm::value_ptr(v));
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat4& m)
{
	auto id = getUniformId(name);
	if (id == -1)
		return;

	glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(m));
}

void ShaderProgram::setUniform(const std::string& name, const std::vector<float>& v)
{
	if (v.empty())
		return;

	auto id = getUniformId(name);
	if (id == -1)
		return;

	glUniform1fv(id, static_cast<GLsizei>(v.size()), reinterpret_cast<const GLfloat*>(v.data()));
}

void ShaderProgram::setUniform(const std::string& name, const std::vector<glm::vec3>& v)
{
	if (v.empty())
		return;

	auto id = getUniformId(name);
	if (id == -1)
		return;

	glUniform3fv(id, static_cast<GLsizei>(v.size()), reinterpret_cast<const GLfloat*>(v.data()));
}