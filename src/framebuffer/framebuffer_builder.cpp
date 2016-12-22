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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dimensions.x, dimensions.y, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	_framebuffer->addTextureAttachment(attachment, textureId);

	_attachments.push_back(attachment);
	return *this;
}

FramebufferBuilder& FramebufferBuilder::withDepthBuffer(const glm::ivec2& dimensions)
{
	if (_framebuffer == nullptr)
		init();

	GLuint renderbuffer;
	glGenRenderbuffers(1, &renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, dimensions.x, dimensions.y);
	_framebuffer->addRenderbufferAttachment(GL_DEPTH_ATTACHMENT, renderbuffer);
	return *this;
}

void FramebufferBuilder::init()
{
	_framebuffer = std::make_shared<Framebuffer>();
	_attachments.clear();
	_framebuffer->activate();
}