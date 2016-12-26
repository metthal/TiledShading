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
		.withTexture(GL_COLOR_ATTACHMENT3, dimensions)
		.withDepthBuffer(dimensions)
		.create();

	return _framebuffer != nullptr;
}

void GBuffer::activate(FramebufferAccess access)
{
	_framebuffer->activate(access);
}

void GBuffer::deactivate(FramebufferAccess access)
{
	_framebuffer->deactivate(access);
}

void GBuffer::activateTextures()
{
	glActiveTexture(GL_TEXTURE0 + getPositionTextureUnit());
	glBindTexture(GL_TEXTURE_2D, getPositionId());

	glActiveTexture(GL_TEXTURE0 + getNormalTextureUnit());
	glBindTexture(GL_TEXTURE_2D, getNormalId());

	glActiveTexture(GL_TEXTURE0 + getAlbedoTextureUnit());
	glBindTexture(GL_TEXTURE_2D, getAlbedoId());

	glActiveTexture(GL_TEXTURE0 + getSpecularTextureUnit());
	glBindTexture(GL_TEXTURE_2D, getSpecularId());
}

GLuint GBuffer::getPositionId() const
{
	return _framebuffer->getAttachmentId(GL_COLOR_ATTACHMENT0);
}

GLuint GBuffer::getNormalId() const
{
	return _framebuffer->getAttachmentId(GL_COLOR_ATTACHMENT1);
}

GLuint GBuffer::getAlbedoId() const
{
	return _framebuffer->getAttachmentId(GL_COLOR_ATTACHMENT2);
}

GLuint GBuffer::getSpecularId() const
{
	return _framebuffer->getAttachmentId(GL_COLOR_ATTACHMENT3);
}

GLuint GBuffer::getDepthId() const
{
	return _framebuffer->getAttachmentId(GL_DEPTH_ATTACHMENT);
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

GLuint GBuffer::getSpecularTextureUnit() const
{
	return 3;
}