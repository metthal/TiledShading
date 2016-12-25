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
	Framebuffer();
	~Framebuffer();

	GLuint getId() const;
	GLuint getAttachmentId(GLuint attachment) const;

	void activate(FramebufferAccess access = FramebufferDrawRead);
	void deactivate(FramebufferAccess access = FramebufferDrawRead);

	bool addAttachment(GLuint attachment, GLuint attachmentId);

private:
	GLuint _id;
	std::unordered_map<GLuint, GLuint> _attachmentTable;
};

#endif