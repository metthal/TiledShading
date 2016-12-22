#include "shaders/deferred_shader_pipeline.h"
#include "window/window.h"

bool DeferredShaderPipeline::init(std::string& error)
{
	auto geometryVs = Shader::load("shaders/deferred_shader.vert", error);
	if (!geometryVs)
		return false;

	auto geometryFs = Shader::load("shaders/deferred_shader.frag", error);
	if (!geometryFs)
		return false;

	auto lightVs = Shader::load("shaders/deferred_shader_light_pass.vert", error);
	if (!lightVs)
		return false;

	auto lightFs = Shader::load("shaders/deferred_shader_light_pass.frag", error);
	if (!lightFs)
		return false;

	_geometryPass = ShaderProgram::link(error, geometryVs, geometryFs);
	if (!_geometryPass)
		return false;

	_lightPass = ShaderProgram::link(error, lightVs, lightFs);
	if (!_lightPass)
		return false;

	_gbuffer = std::make_shared<GBuffer>();
	if (!_gbuffer->init({800, 600}))
		return false;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	return true;
}

void DeferredShaderPipeline::run(const Scene* scene)
{
	auto camera = scene->getCamera();

	std::vector<glm::vec3> lights;
	for (const auto& light : scene->getLights())
		lights.push_back(light->getPosition());

	_gbuffer->activate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_geometryPass->activate();
	_geometryPass->setUniform("viewProject", camera->getViewTransform());
	for (const auto& object : *scene)
	{
		_geometryPass->setUniform("model", object->getTransform());
		object->getMesh()->render();
	}

	_gbuffer->deactivate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_lightPass->activate();
	_gbuffer->activateTextures();
	_lightPass->setUniform("viewProject", camera->getViewTransform());
	_lightPass->setUniform("gbufferPos", _gbuffer->getPositionTextureUnit());
	_lightPass->setUniform("gbufferNormal", _gbuffer->getNormalTextureUnit());
	_lightPass->setUniform("gbufferAlbedo", _gbuffer->getAlbedoTextureUnit());
	_lightPass->setUniform("lightsCount", static_cast<GLint>(scene->getNumberOfLights()));
	_lightPass->setUniform("lights", lights);
	for (const auto& object : *scene)
	{
		_lightPass->setUniform("model", object->getTransform());
		object->getMesh()->render();
	}
}