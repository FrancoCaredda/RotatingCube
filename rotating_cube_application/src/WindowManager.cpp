#include "WindowManager.h"

#include <stdexcept>

WindowManager WindowManager::s_WindowManager;

WindowManager& WindowManager::GetInstance()
{
    return s_WindowManager;
}

void WindowManager::Init()
{
    if (!glfwInit())
        throw std::runtime_error::exception("GLFW hasn't been initialized!");
}

void WindowManager::Terminate()
{
    for (auto window : m_Windows)
        delete window;

    m_Windows.clear();
    glfwTerminate();
}

WindowManager::~WindowManager()
{
    if (m_Windows.size() > 0)
        Terminate();
}
