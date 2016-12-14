#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "mesh/mesh.h"

Mesh::Mesh(const std::vector<glm::vec3>& vertices, const std::vector<std::uint32_t>& indices, const std::vector<glm::vec3>& normals)
	: _vertices(vertices), _indices(indices), _normals(normals)
{
	init();
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
	glDeleteVertexArrays(1, &_vao);
}

Mesh Mesh::loadFromFile(const std::string& fileName)
{
	Assimp::Importer importer;
	auto scene = importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals);
	if (!scene)
		return {{}, {}, {}};

	auto mesh = scene->mMeshes[0];
	std::vector<glm::vec3> vertices;
	std::vector<std::uint32_t> indices;
	std::vector<glm::vec3> normals;

	vertices.reserve(mesh->mNumVertices);
	normals.reserve(mesh->mNumVertices);

	for (std::size_t i = 0; i < mesh->mNumVertices; ++i)
	{
		auto vertex = mesh->mVertices[i];
		auto normal = mesh->mNormals[i];

		vertices.emplace_back(vertex.x, vertex.y, vertex.z);
		normals.emplace_back(normal.x, normal.y, normal.z);
	}

	for (std::size_t i = 0; i < mesh->mNumFaces; ++i)
	{
		const auto& face = mesh->mFaces[i];
		for (std::size_t j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	return { vertices, indices, normals };
}

GLuint Mesh::getVertexId() const
{
	return _vao;
}

GLuint Mesh::getIndexId() const
{
	return _ebo;
}

std::size_t Mesh::getNumberOfVertices() const
{
	return _vertices.size();
}

std::size_t Mesh::getNumberOfIndices() const
{
	return _indices.size();
}

void Mesh::render()
{
	glBindVertexArray(_vao);
	if (_indices.empty())
		glDrawArrays(GL_TRIANGLES, 0, _vertices.size() * 3);
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
	}
}

void Mesh::init()
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);

	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * 2 * 3 * sizeof(float), nullptr, GL_STATIC_DRAW);
	auto buffer = static_cast<glm::vec3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	for (std::size_t i = 0; i < _vertices.size(); ++i)
	{
		buffer[2 * i + 0] = _vertices[i];
		buffer[2 * i + 1] = _normals[i];
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(std::uint32_t), _indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexArrayAttrib(_vao, 0);
	glEnableVertexArrayAttrib(_vao, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}