#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "Specification.h"
#include "Window.h"

class Application
{
public:
	static Application& GetInstance();

	void Init(ApplicationSpec& appSpec);
	void Run();

	void Terminate();
private:
	Application() = default;
	static Application s_Application;

	Window* m_Window;
};

#endif // !_APPLICATION_H
