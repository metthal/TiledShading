#include "shaders/tiled_deferred_shader_pipeline.h"
#include "window/window.h"

const std::size_t WorkGroupSize = 32;

bool TiledDeferredShaderPipeline::init(const Window* window, std::string& error)
{
	_fullScreenQuad = Mesh::load("quad.obj");
	if (_fullScreenQuad == nullptr)
	{
		error = "Unable to load fullscreen quad";
		return false;
	}

	auto geometryVs = Shader::loadFile("deferred_shader_geo_pass.vert", error);
	if (!geometryVs)
		return false;

	auto geometryFs = Shader::loadFile("deferred_shader_geo_pass.frag", error);
	if (!geometryFs)
		return false;

	auto lightVs = Shader::loadFile("tiled_deferred_shader_light_pass.vert", error);
	if (!lightVs)
		return false;

	auto lightFs = Shader::loadFile("tiled_deferred_shader_light_pass.frag", error);
	if (!lightFs)
		return false;

	auto tileCs = Shader::loadFile("tiled_deferred_shader_light_pass.comp", error);
	if (!tileCs)
		return false;

	auto tileCsDebug = Shader::loadFile("tiled_deferred_shader_light_pass_debug.comp", error);
	if (!tileCsDebug)
		return false;

	_geometryPass = ShaderProgram::link(error, geometryVs, geometryFs);
	if (!_geometryPass)
		return false;

	_lightPass = ShaderProgram::link(error, lightVs, lightFs);
	if (!_lightPass)
		return false;

	_tilePass = ShaderProgram::link(error, tileCs);
	if (!_tilePass)
		return false;

	_tilePassDebug = ShaderProgram::link(error, tileCsDebug);
	if (!_tilePassDebug)
		return false;

	_gbuffer = std::make_shared<GBuffer>();
	if (!_gbuffer->init(window->getDimensions()))
		return false;

	glGenTextures(1, &_tiledLights);
	glBindTexture(GL_TEXTURE_2D, _tiledLights);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, window->getDimensions().x, window->getDimensions().y, 0, GL_RGBA, GL_FLOAT, nullptr);

	_lightPass->activate();
	_lightPass->setUniformBlockBindPoint("Lights", 0);
	glBindBuffer(GL_UNIFORM_BUFFER, window->getScene()->getLightsBufferId());
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, window->getScene()->getLightsBufferId());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	_lightPass->deactivate();

	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	return true;
}

void TiledDeferredShaderPipeline::run(Window* window, std::uint32_t diff)
{
	auto windowSize = window->getDimensions();
	auto windowWidthHalf = windowSize.x / 2;
	auto windowHeightHalf = windowSize.y / 2;
	auto scene = window->getScene();
	auto camera = scene->getCamera();

	glEnable(GL_CULL_FACE);
	_gbuffer->activate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_geometryPass->activate();
	_geometryPass->setUniform("viewProject", camera->getViewProjectTransform());
	for (const auto& object : *scene)
	{
		_geometryPass->setUniform("model", object->getTransform());
		_geometryPass->setUniform("objectColor", object->getColor());
		object->getMesh()->render();
	}

	glDisable(GL_CULL_FACE);
	_gbuffer->deactivate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	auto tilePass = isDebug() ? _tilePassDebug : _tilePass;
	tilePass->activate();
	_gbuffer->activateTextures();
	tilePass->setUniform("cameraPos", camera->getPosition());
	tilePass->setUniform("gbufferPos", _gbuffer->getPositionTextureUnit());
	tilePass->setUniform("gbufferNormal", _gbuffer->getNormalTextureUnit());
	tilePass->setUniform("gbufferAlbedo", _gbuffer->getAlbedoTextureUnit());
	tilePass->setUniform("gbufferSpecular", _gbuffer->getSpecularTextureUnit());
	tilePass->setUniform("gbufferDepth", _gbuffer->getDepthTextureUnit());
	tilePass->setUniform("project", camera->getProjectTransform());
	tilePass->setUniform("view", camera->getViewTransform());
	tilePass->setUniform("lightsCount", static_cast<GLint>(scene->getNumberOfLights()));
	glBindImageTexture(5, _tiledLights, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glDispatchCompute((1366 / WorkGroupSize) + (1366 & (WorkGroupSize - 1)), (768 / WorkGroupSize) + (768 & (WorkGroupSize - 1)), 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	_lightPass->activate();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _tiledLights);
	_lightPass->setUniform("tiledLights", 0);
	_fullScreenQuad->render();
}