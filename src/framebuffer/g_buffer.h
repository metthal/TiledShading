#ifndef TILED_SHADING_FRAMEBUFFER_G_BUFFER_H
#define TILED_SHADING_FRAMEBUFFER_G_BUFFER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "framebuffer/framebuffer.h"

class GBuffer
{
public:
	bool init(const glm::ivec2& dimensions);

	void activate();
	void deactivate();
	void activateTextures();

	GLuint getPositionId() const;
	GLuint getNormalId() const;
	GLuint getAlbedoId() const;

	GLuint getPositionTextureUnit() const;
	GLuint getNormalTextureUnit() const;
	GLuint getAlbedoTextureUnit() const;

private:
	std::shared_ptr<Framebuffer> _framebuffer;
};

#endif