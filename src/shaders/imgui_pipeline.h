#ifndef TILED_SHADING_SHADERS_IMGUI_PIPELINE_H
#define TILED_SHADING_SHADERS_IMGUI_PIPELINE_H

#include <memory>
#include <string>

#include <glm/glm.hpp>
#include <imgui.h>
#include <SDL.h>

#include "shaders/pipeline.h"
#include "shaders/shader_program.h"

class ImguiPipeline : public Pipeline
{
public:
	ImguiPipeline();
	~ImguiPipeline();

	virtual bool init(const Window* window, std::string& error) override;
	virtual void run(Window* window, std::uint32_t diff) override;

	GLuint getVertexArrayId() const;
	GLuint getVertexBufferId() const;
	GLuint getElementBufferId() const;
	const std::shared_ptr<ShaderProgram>& getImguiProgram() const;

	void handleEvent(const SDL_Event& event) const;

private:
	static void imguiDrawCallback(ImDrawData* data);

	std::shared_ptr<ShaderProgram> _imguiProgram;
	GLuint _vao, _vbo, _ebo, _fontTexture;
};

#endif