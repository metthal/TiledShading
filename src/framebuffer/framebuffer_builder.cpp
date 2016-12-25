#include "framebuffer/framebuffer_builder.h"

std::shared_ptr<Framebuffer> FramebufferBuilder::create()
{
	glDrawBuffers(static_cast<GLsizei>(_attachments.size()), _attachments.data());

	auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		_framebuffer->deactivate();
		_framebuffer = nullptr;
		return nullptr;
	}

	_framebuffer->deactivate();

	auto tmp = _framebuffer;
	_framebuffer = nullptr;
	return tmp;
}

FramebufferBuilder& FramebufferBuilder::withTexture(GLuint attachment, const glm::ivec2& dimensions)
{
	if (_framebuffer == nullptr)
		init();

	GLuint textureId;
	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, dimensions.x, dimensions.y, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	_framebuffer->addAttachment(attachment, textureId);

	_attachments.push_back(attachment);
	return *this;
}

FramebufferBuilder& FramebufferBuilder::withDepthBuffer(const glm::ivec2& dimensions)
{
	if (_framebuffer == nullptr)
		init();

	GLuint textureId;
	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, dimensions.x, dimensions.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	_framebuffer->addAttachment(GL_DEPTH_ATTACHMENT, textureId);
	return *this;
}

void FramebufferBuilder::init()
{
	_framebuffer = std::make_shared<Framebuffer>();
	_attachments.clear();
	_framebuffer->activate();
}