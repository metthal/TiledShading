#ifndef TILED_SHADING_MESH_MESH_H
#define TILED_SHADING_MESH_MESH_H

#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>

class ShaderProgram;

class Mesh
{
public:
	Mesh(const std::vector<glm::vec3>& vertices, const std::vector<std::uint32_t>& indices, const std::vector<glm::vec3>& normals);
	~Mesh();

	static Mesh loadFromFile(const std::string& fileName);

	GLuint getVertexId() const;
	GLuint getIndexId() const;
	std::size_t getNumberOfVertices() const;
	std::size_t getNumberOfIndices() const;

	void render();

private:
	void init();

	std::vector<glm::vec3> _vertices;
	std::vector<std::uint32_t> _indices;
	std::vector<glm::vec3> _normals;
	GLuint _vao;
	GLuint _vbo, _ebo;
};

#endif