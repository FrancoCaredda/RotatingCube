#ifndef _WINDOW_MANAGER_H
#define _WINDOW_MANAGER_H

#include "Window.h"

#include <GLFW/glfw3.h>

#include <vector>
#include <type_traits>

class WindowManager
{
public:
	static WindowManager& GetInstance();

	void Init();
	void Terminate();

	template<typename WindowType>
	Window* CreateWindowInstance(int width, int height, const std::string& title)
	{
		if constexpr (std::is_base_of<Window, WindowType>())
		{
			m_Windows.push_back(new WindowType(width, height, title));
			return m_Windows[m_Windows.size() - 1];
		}
		
		return nullptr;
	}

	~WindowManager();
private:
	WindowManager() = default;
	static WindowManager s_WindowManager;

	std::vector<Window*> m_Windows;
};

#endif // !_WINDOW_MANAGER_H
