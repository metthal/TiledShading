#ifndef TILED_SHADING_DEFERRED_SHADING_G_BUFFER_H
#define TILED_SHADING_DEFERRED_SHADING_G_BUFFER_H

#include <GL/glew.h>

class GBuffer
{
public:
	GBuffer();
	~GBuffer();

	bool init();

	void useFramebuffer();
	void useTextures();

	GLuint getId() const;
	GLuint getPositionId() const;
	GLuint getNormalId() const;
	GLuint getAlbedoId() const;

private:

	GLuint _fbo;
	GLuint _posTexture, _normalTexture, _albedoTexture, _depthRb;

};

#endif