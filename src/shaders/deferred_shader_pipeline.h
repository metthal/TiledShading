#ifndef TILED_SHADING_SHADERS_DEFERRED_SHADER_PIPELINE_H
#define TILED_SHADING_SHADERS_DEFERRED_SHADER_PIPELINE_H

#include <memory>

#include "framebuffer/g_buffer.h"
#include "mesh/mesh.h"
#include "shaders/pipeline.h"
#include "shaders/shader_program.h"

class DeferredShaderPipeline : public Pipeline
{
public:
	virtual bool init(const Window* window, std::string& error) override;
	virtual void run(const Scene* scene) override;

private:
	std::shared_ptr<GBuffer> _gbuffer;
	std::shared_ptr<ShaderProgram> _geometryPass;
	std::shared_ptr<ShaderProgram> _lightPass;
	std::shared_ptr<Mesh> _fullScreenQuad;
};

#endif