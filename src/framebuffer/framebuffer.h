#ifndef TILED_SHADING_FRAMEBUFFER_FRAMEBUFFER_H
#define TILED_SHADING_FRAMEBUFFER_FRAMEBUFFER_H

#include <memory>
#include <unordered_map>

#include <GL/glew.h>

enum FramebufferAccess
{
	FramebufferDraw = GL_DRAW_FRAMEBUFFER,
	FramebufferRead = GL_READ_FRAMEBUFFER,
	FramebufferDrawRead = GL_FRAMEBUFFER
};

class Framebuffer
{
public:
	using TypeId = std::tuple<GLuint, GLuint>;

	Framebuffer();
	~Framebuffer();

	GLuint getId() const;
	TypeId getAttachmentTypeId(GLuint attachment) const;

	void activate(FramebufferAccess access = FramebufferDrawRead);
	void deactivate(FramebufferAccess access = FramebufferDrawRead);

	bool addTextureAttachment(GLuint attachment, GLuint attachmentId);
	bool addRenderbufferAttachment(GLuint attachment, GLuint attachmentId);


private:
	GLuint _id;
	std::unordered_map<GLuint, TypeId> _attachmentTable;
};

#endif