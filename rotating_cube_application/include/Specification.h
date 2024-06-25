#ifndef _SPECIFICATION_H
#define _SPECIFICATION_H

#include <string>
#include <vector>

struct ApplicationSpec
{
	std::string AppName;
	int Major, Minor, Patch;
};

void InitApplicationSpec(ApplicationSpec& outApplicationSpec, const std::string& appName,
	int major, int minor, int patch);

struct RendererSpec
{
	int WindowWidth, WindowHeight;

	std::vector<const char*> Layers;
	std::vector<const char*> Extensions;
};

void InitRendererSpec(RendererSpec& outRendererSpec, int width, int height);


#endif // !_SPECIFICATION_H
