#ifndef TILED_SHADING_FRAMEBUFFER_FRAMEBUFFER_BUILDER_H
#define TILED_SHADING_FRAMEBUFFER_FRAMEBUFFER_BUILDER_H

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "framebuffer/framebuffer.h"

class FramebufferBuilder
{
public:
	std::shared_ptr<Framebuffer> create();

	FramebufferBuilder& withTexture(GLuint attachment, const glm::ivec2& dimensions);
	FramebufferBuilder& withDepthBuffer(const glm::ivec2& dimensions);

private:
	void init();

	std::shared_ptr<Framebuffer> _framebuffer;
	std::vector<GLuint> _attachments;
};

#endif