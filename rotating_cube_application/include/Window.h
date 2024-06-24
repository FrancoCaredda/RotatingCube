#ifndef _WINDOW_H
#define _WINDOW_H

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <string>

class Window
{
public:
	Window(VkInstance instance, int width, int height, const std::string& title);
	Window(Window&& window) noexcept = delete;
	Window(const Window&) = delete;
	~Window();

	void Update(float deltaTime);
	void Poll();

	inline GLFWwindow* GetNative() const noexcept { return m_Handle; }
	inline VkSurfaceKHR GetSurface() const noexcept { return m_Surface; }

	inline bool IsClosed() const noexcept { return glfwWindowShouldClose(m_Handle); }
private:
	GLFWwindow* m_Handle = nullptr;
	VkSurfaceKHR m_Surface = nullptr;

	VkInstance m_Instance = nullptr;

	int m_Width = 0,
		m_Height = 0;

	std::string m_Title{};
};

#endif // !_WINDOW_H
