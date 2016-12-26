#ifndef TILED_SHADING_SHADERS_TILED_DEFERRED_SHADER_PIPELINE_H
#define TILED_SHADING_SHADERS_TILED_DEFERRED_SHADER_PIPELINE_H

#include <memory>
#include <string>

#include "framebuffer/g_buffer.h"
#include "mesh/mesh.h"
#include "shaders/pipeline.h"
#include "shaders/shader_program.h"

class TiledDeferredShaderPipeline : public Pipeline
{
public:
	virtual bool init(const Window* window, std::string& error) override;
	virtual void run(Window* window, std::uint32_t diff) override;

private:
	std::shared_ptr<GBuffer> _gbuffer;
	std::shared_ptr<ShaderProgram> _geometryPass;
	std::shared_ptr<ShaderProgram> _lightPass;
	std::shared_ptr<ShaderProgram> _tilePass;
	std::shared_ptr<ShaderProgram> _tilePassDebug;
	std::shared_ptr<Mesh> _fullScreenQuad;
	GLuint _tiledLights;
};

#endif