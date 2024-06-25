#ifndef _WINDOW_H
#define _WINDOW_H

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <string>

#define GENERATE_WINDOW_BODY(class_name) class_name(int width, int height, const std::string& title) : Window(width, height, title) {} \
										 class_name(class_name&& window) noexcept : Window(std::move(window)) { } \
										 class_name(const class_name&) = delete;\
										 virtual ~class_name() {}

class Window
{
public:
	Window(int width, int height, const std::string& title);
	Window(Window&& window) noexcept;
	Window(const Window&) = delete;
	virtual ~Window();

	virtual void Update(float deltaTime) = 0;
	void Poll();

	inline GLFWwindow* GetNative() const noexcept { return m_Handle; }
	inline bool IsClosed() const noexcept { return glfwWindowShouldClose(m_Handle); }
private:
	GLFWwindow* m_Handle = nullptr;

	int m_Width = 0,
		m_Height = 0;

	std::string m_Title{};
};

#endif // !_WINDOW_H
