#include <array>

#include "deferred_shading/g_buffer.h"

GBuffer::GBuffer()
{
}

GBuffer::~GBuffer()
{
	glDeleteTextures(1, &_posTexture);
	glDeleteTextures(1, &_normalTexture);
	glDeleteTextures(1, &_albedoTexture);
	glDeleteRenderbuffers(1, &_depthRb);
	glDeleteFramebuffers(1, &_fbo);
}

void GBuffer::useFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
}

void GBuffer::useTextures()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _posTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _normalTexture);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _albedoTexture);
}

GLuint GBuffer::getId() const
{
	return _fbo;
}

GLuint GBuffer::getPositionId() const
{
	return _posTexture;
}

GLuint GBuffer::getNormalId() const
{
	return _normalTexture;
}

GLuint GBuffer::getAlbedoId() const
{
	return _albedoTexture;
}

bool GBuffer::init()
{
	glCreateFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	std::array<GLuint, 3> textures;
	glGenTextures(3, textures.data());
	_posTexture = textures[0];
	_normalTexture = textures[1];
	_albedoTexture = textures[2];

	glBindTexture(GL_TEXTURE_2D, _posTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _posTexture, 0);

	glBindTexture(GL_TEXTURE_2D, _normalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _normalTexture, 0);

	glBindTexture(GL_TEXTURE_2D, _albedoTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, _albedoTexture, 0);

	glGenRenderbuffers(1, &_depthRb);
	glBindRenderbuffer(GL_RENDERBUFFER, _depthRb);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRb);

	std::array<GLuint, 3> drawBuffers = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(drawBuffers.size(), drawBuffers.data());

	auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}