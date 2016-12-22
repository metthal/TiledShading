#include <array>

#include "framebuffer/framebuffer_builder.h"
#include "framebuffer/g_buffer.h"

bool GBuffer::init(const glm::ivec2& dimensions)
{
	FramebufferBuilder newFramebuffer;
	_framebuffer = newFramebuffer
		.withTexture(GL_COLOR_ATTACHMENT0, dimensions)
		.withTexture(GL_COLOR_ATTACHMENT1, dimensions)
		.withTexture(GL_COLOR_ATTACHMENT2, dimensions)
		.withDepthBuffer(dimensions)
		.create();

	return _framebuffer != nullptr;
}

void GBuffer::activate()
{
	_framebuffer->activate();
}

void GBuffer::deactivate()
{
	_framebuffer->deactivate();
}

void GBuffer::activateTextures()
{
	glActiveTexture(GL_TEXTURE0 + getPositionTextureUnit());
	glBindTexture(GL_TEXTURE_2D, getPositionId());

	glActiveTexture(GL_TEXTURE0 + getNormalTextureUnit());
	glBindTexture(GL_TEXTURE_2D, getNormalId());

	glActiveTexture(GL_TEXTURE0 + getAlbedoTextureUnit());
	glBindTexture(GL_TEXTURE_2D, getAlbedoId());
}

GLuint GBuffer::getPositionId() const
{
	return std::get<1>(_framebuffer->getAttachmentTypeId(GL_COLOR_ATTACHMENT0));
}

GLuint GBuffer::getNormalId() const
{
	return std::get<1>(_framebuffer->getAttachmentTypeId(GL_COLOR_ATTACHMENT1));
}

GLuint GBuffer::getAlbedoId() const
{
	return std::get<1>(_framebuffer->getAttachmentTypeId(GL_COLOR_ATTACHMENT2));
}

GLuint GBuffer::getPositionTextureUnit() const
{
	return 0;
}

GLuint GBuffer::getNormalTextureUnit() const
{
	return 1;
}

GLuint GBuffer::getAlbedoTextureUnit() const
{
	return 2;
}