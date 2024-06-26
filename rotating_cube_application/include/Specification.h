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

	std::vector<const char*> Extensions;
	std::vector<const char*> Layers;
};

void InitRendererSpec(RendererSpec& outRendererSpec, int width, int height);

struct DeviceSpec
{
	std::vector<const char*> Extensions;
	std::vector<const char*> Layers;
};

#endif // !_SPECIFICATION_H
