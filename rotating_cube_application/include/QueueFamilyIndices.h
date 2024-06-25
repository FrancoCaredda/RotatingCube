#ifndef _QUEUE_FAMILY_INDICES
#define _QUEUE_FAMILY_INDICES

#include <optional>

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsQueue;
	std::optional<uint32_t> presentationQueue;
};

#endif // !_QUEUE_FAMILY_INDICES
