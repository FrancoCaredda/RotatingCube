#include "Window.h"

#include <iostream>
#include <stdexcept>

Window::Window(int width, int height, const std::string& title)
	: m_Width(width), m_Height(height), m_Title(title)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_Handle = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);

	if (!m_Handle)
		throw std::runtime_error::exception("Window hasn't been created!");
}

Window::Window(Window&& window) noexcept
{
	if (m_Handle)
		glfwDestroyWindow(m_Handle);

	m_Handle = window.m_Handle;
	m_Width = window.m_Width;
	m_Height = window.m_Height;
	m_Title = window.m_Title;

	window.m_Handle = nullptr;
	window.m_Width = 0;
	window.m_Height = 0;
	window.m_Title.clear();
}

Window::~Window()
{
	if (m_Handle)
		glfwDestroyWindow(m_Handle);
}

void Window::Poll()
{
	glfwPollEvents();
}
