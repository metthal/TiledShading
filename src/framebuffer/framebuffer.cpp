#include "framebuffer/framebuffer.h"

Framebuffer::Framebuffer()
{
	glGenFramebuffers(1, &_id);
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &_id);
	for (const auto& keyValue : _attachmentTable)
	{
		auto id = keyValue.second;
		glDeleteTextures(1, &id);
	}
}

GLuint Framebuffer::getId() const
{
	return _id;
}

GLuint Framebuffer::getAttachmentId(GLuint attachment) const
{
	auto itr = _attachmentTable.find(attachment);
	if (itr == _attachmentTable.end())
		return 0;

	return itr->second;
}

void Framebuffer::activate(FramebufferAccess access)
{
	glBindFramebuffer(access, _id);
}

void Framebuffer::deactivate(FramebufferAccess access)
{
	glBindFramebuffer(access, 0);
}

bool Framebuffer::addAttachment(GLuint attachment, GLuint attachmentId)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, attachmentId, 0);
	return _attachmentTable.insert({ attachment, attachmentId }).second;
}