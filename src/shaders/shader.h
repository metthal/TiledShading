#ifndef TILED_SHADING_SHADERS_SHADER_H
#define TILED_SHADING_SHADERS_SHADER_H

#include <cstdint>
#include <string>

#include <GL/glew.h>

class Shader
{
public:
	static Shader loadVertexShader(const std::string& fileName);
	static Shader loadFragmentShader(const std::string& fileName);

	operator bool() const;

	std::uint32_t getType() const;
	GLuint getId() const;
	const std::string& getError() const;

private:
	Shader(const std::string& error);
	Shader(std::uint32_t type, GLuint id);

	static Shader loadShader(std::uint32_t type, const std::string& fileName);

	std::uint32_t _type;
	GLuint _id;
	std::string _error;
};

#endif