#ifndef TILED_SHADING_SHADERS_SHADER_H
#define TILED_SHADING_SHADERS_SHADER_H

#include <cstdint>
#include <memory>
#include <string>

#include <GL/glew.h>

class Shader
{
public:
	Shader(std::uint32_t type, GLuint id);
	~Shader();


	static std::shared_ptr<Shader> loadString(GLuint type, const std::string& shaderSource, std::string& error);
	static std::shared_ptr<Shader> loadFile(const std::string& fileName, std::string& error);

	std::uint32_t getType() const;
	GLuint getId() const;

private:
	std::uint32_t _type;
	GLuint _id;
};

#endif