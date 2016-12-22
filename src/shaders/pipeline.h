#ifndef TILED_SHADING_SHADERS_PIPELINE_H
#define TILED_SHADING_SHADERS_PIPELINE_H

class Scene;

class Pipeline
{
public:
	virtual ~Pipeline() = default;

	virtual bool init(std::string& error) = 0;
	virtual void run(const Scene* scene) = 0;
};

#endif