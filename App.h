#pragma once
#include "Window.h"
#include <memory>
#include "Graphics.h"

class App
{
public:
	int Run();
private:
	Window window;
	std::unique_ptr<Graphics>graphics;
};

