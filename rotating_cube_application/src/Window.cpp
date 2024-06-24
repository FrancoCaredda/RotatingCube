#include "Window.h"

#include <iostream>
#include <stdexcept>

Window::Window(VkInstance instance, int width, int height, const std::string& title)
	: m_Width(width), m_Height(height), m_Title(title), m_Instance(instance)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_Handle = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);

	if (!m_Handle)
		throw std::runtime_error::exception("Window hasn't been created!");

	
	if (glfwCreateWindowSurface(instance, m_Handle, nullptr, &m_Surface) != VK_SUCCESS)
		throw std::runtime_error::exception("Surface hasn't been created!");
}

Window::~Window()
{
	vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
	glfwDestroyWindow(m_Handle);
}

void Window::Update(float deltaTime)
{
}

void Window::Poll()
{
	glfwPollEvents();
}
