#ifndef TILED_SHADING_SHADER_PROGRAM_H
#define TILED_SHADING_SHADER_PROGRAM_H

#include <memory>
#include <unordered_map>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "shaders/shader.h"

struct AttributeInfo
{
	GLint id;
};

struct UniformInfo
{
	GLint id;
	bool array;
};

class ShaderProgram
{
public:
	ShaderProgram(GLuint id, std::vector<std::shared_ptr<Shader>>&& shaders);
	~ShaderProgram();

	template <typename... Shaders>
	static std::shared_ptr<ShaderProgram> link(std::string& error, const Shaders&... shaders)
	{
		auto id = glCreateProgram();
		std::vector<std::shared_ptr<Shader>> shaderPtrs;
		return linkImpl(id, std::move(shaderPtrs), error, shaders...);
	}

	void activate();
	void deactivate();

	GLuint getId() const;
	GLint getNumberOfAttributes() const;
	GLint getNumberOfUniforms() const;
	GLint getAttributeId(const std::string& name) const;
	GLint getUniformId(const std::string& name) const;

	void setUniform(const std::string& name, GLint value);
	void setUniform(const std::string& name, const glm::vec3& v);
	void setUniform(const std::string& name, const glm::mat4& m);
	void setUniform(const std::string& name, const std::vector<float>& v);
	void setUniform(const std::string& name, const std::vector<glm::vec3>& v);

private:
	static std::shared_ptr<ShaderProgram> linkImpl(GLuint id, std::vector<std::shared_ptr<Shader>>&& shaderPtrs, std::string& error)
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

			error = std::string(errorData.begin(), errorData.end());
			return nullptr;
		}

		return std::make_shared<ShaderProgram>(id, std::move(shaderPtrs));
	}

	template <typename... Shaders>
	static std::shared_ptr<ShaderProgram> linkImpl(GLuint id, std::vector<std::shared_ptr<Shader>>&& shaderPtrs, std::string& error,
		const std::shared_ptr<Shader>& shader, const Shaders&... shaders)
	{
		glAttachShader(id, shader->getId());
		shaderPtrs.push_back(std::move(shader));
		return linkImpl(id, std::move(shaderPtrs), error, shaders...);
	}

	GLuint _id;
	GLint _numAttributes;
	GLint _numUniforms;
	std::vector<std::shared_ptr<Shader>> _shaders;
	std::unordered_map<std::string, AttributeInfo> _attributes;
	std::unordered_map<std::string, UniformInfo> _uniforms;
};

#endif