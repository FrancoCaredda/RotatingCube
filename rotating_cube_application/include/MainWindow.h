#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H

#include "Window.h"

class MainWindow : public Window
{
public:
	GENERATE_WINDOW_BODY(MainWindow)

	void Update(float deltaTime) override;
};

#endif // !_MAIN_WINDOW_H
