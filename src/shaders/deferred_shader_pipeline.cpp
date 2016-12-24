#include "shaders/deferred_shader_pipeline.h"
#include "window/window.h"

bool DeferredShaderPipeline::init(const Window* window, std::string& error)
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

	auto lightVs = Shader::loadFile("deferred_shader_light_pass.vert", error);
	if (!lightVs)
		return false;

	auto lightFs = Shader::loadFile("deferred_shader_light_pass.frag", error);
	if (!lightFs)
		return false;

	_geometryPass = ShaderProgram::link(error, geometryVs, geometryFs);
	if (!_geometryPass)
		return false;

	_lightPass = ShaderProgram::link(error, lightVs, lightFs);
	if (!_lightPass)
		return false;

	_gbuffer = std::make_shared<GBuffer>();
	if (!_gbuffer->init(window->getDimensions()))
		return false;

	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	return true;
}

void DeferredShaderPipeline::run(const Window* window, std::uint32_t diff)
{
	auto scene = window->getScene();
	auto camera = scene->getCamera();

	std::vector<glm::vec3> lightsPos, lightsIntensity;
	std::vector<float> lightsAttenuation;
	for (const auto& light : scene->getLights())
	{
		lightsPos.push_back(light->getPosition());
		lightsIntensity.push_back(light->getIntensity());
		lightsAttenuation.push_back(light->getAttenuation());
	}

	glEnable(GL_CULL_FACE);
	_gbuffer->activate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_geometryPass->activate();
	_geometryPass->setUniform("viewProject", camera->getViewTransform());
	for (const auto& object : *scene)
	{
		_geometryPass->setUniform("model", object->getTransform());
		_geometryPass->setUniform("objectColor", object->getColor());
		object->getMesh()->render();
	}

	glDisable(GL_CULL_FACE);
	_gbuffer->deactivate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_lightPass->activate();
	_gbuffer->activateTextures();
	_lightPass->setUniform("gbufferPos", _gbuffer->getPositionTextureUnit());
	_lightPass->setUniform("gbufferNormal", _gbuffer->getNormalTextureUnit());
	_lightPass->setUniform("gbufferAlbedo", _gbuffer->getAlbedoTextureUnit());
	_lightPass->setUniform("lightsCount", static_cast<GLint>(scene->getNumberOfLights()));
	_lightPass->setUniform("lightsPos", lightsPos);
	_lightPass->setUniform("lightsIntensity", lightsIntensity);
	_lightPass->setUniform("lightsAttenuation", lightsAttenuation);
	_fullScreenQuad->render();
}