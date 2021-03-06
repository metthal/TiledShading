#ifndef TILED_SHADING_SHADERS_PIPELINE_H
#define TILED_SHADING_SHADERS_PIPELINE_H

class Window;

class Pipeline
{
public:
	virtual ~Pipeline() = default;

	virtual bool init(const Window* window, std::string& error) = 0;
	virtual void run(Window* window, std::uint32_t diff) = 0;

	bool isDebug() const { return _debug; }
	void setDebug(bool set) { _debug = set; }

private:
	bool _debug = false;
};

#endif