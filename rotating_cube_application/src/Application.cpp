#include "Application.h"
#include "Renderer.h"
#include "WindowManager.h"
#include "MainWindow.h"

#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>

Application Application::s_Application;

Application& Application::GetInstance()
{
	return s_Application;
}

void Application::Init(ApplicationSpec& appSpec)
{
	WindowManager& windowManager = WindowManager::GetInstance();
	windowManager.Init();

	m_Window = windowManager.CreateWindowInstance<MainWindow>(800, 600, appSpec.AppName);

	RendererSpec rendererSpec{};
	InitRendererSpec(rendererSpec, 800, 600);

	rendererSpec.Extensions.push_back("VK_KHR_surface");
	rendererSpec.Extensions.push_back("VK_KHR_win32_surface");

#ifdef _DEBUG
	rendererSpec.Extensions.push_back("VK_EXT_debug_utils");
	rendererSpec.Layers.push_back("VK_LAYER_KHRONOS_validation");
#endif // _DEBUG

	Renderer& renderer = Renderer::GetRenderer();
	renderer.Init(appSpec, rendererSpec, m_Window);
}

void Application::Run()
{
	auto now = std::chrono::system_clock::now();
	auto previous = now;

	while (!m_Window->IsClosed())
	{
		now = std::chrono::system_clock::now();
		auto deltaTime = std::chrono::
			duration_cast<std::chrono::duration<double>>(
				now - previous
			);

		previous = now;

		m_Window->Update(deltaTime.count());
		m_Window->Poll();
	}
}

void Application::Terminate()
{
	Renderer& renderer = Renderer::GetRenderer();
	renderer.Terminate();

	WindowManager& windowManager = WindowManager::GetInstance();
	windowManager.Terminate();
}
