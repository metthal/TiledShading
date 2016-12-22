#include "framebuffer/framebuffer.h"

Framebuffer::Framebuffer()
{
	glGenFramebuffers(1, &_id);
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &_id);
	for (auto& keyValue : _attachmentTable)
	{
		auto type = std::get<0>(keyValue.second);
		auto id = std::get<1>(keyValue.second);

		if (type == GL_TEXTURE_2D)
			glDeleteTextures(1, &id);
		else if (type == GL_RENDERBUFFER)
			glDeleteRenderbuffers(1, &id);
	}
}

GLuint Framebuffer::getId() const
{
	return _id;
}

Framebuffer::TypeId Framebuffer::getAttachmentTypeId(GLuint attachment) const
{
	auto itr = _attachmentTable.find(attachment);
	if (itr == _attachmentTable.end())
		return { 0, 0 };

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

bool Framebuffer::addTextureAttachment(GLuint attachment, GLuint attachmentId)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, attachmentId, 0);
	return _attachmentTable.insert({ attachment, { GL_TEXTURE_2D, attachmentId } }).second;
}

bool Framebuffer::addRenderbufferAttachment(GLuint attachment, GLuint attachmentId)
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, attachmentId);
	return _attachmentTable.insert({ attachment, { GL_RENDERBUFFER, attachmentId } }).second;
}