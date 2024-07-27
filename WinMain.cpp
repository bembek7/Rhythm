#include "Graphics.h"
#include <chrono>
#include "Window.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR pCmdLine, int nCmdShow)
{
	Window window = Window(hInstance);

	Graphics graphics = Graphics(window.GetHWNDRef(), window.GetWindowWidth(), window.GetWindowHeight());
	
	auto last = std::chrono::steady_clock::now();
	auto start = std::chrono::steady_clock::now();

	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}

		const float deltaTime = std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
		last = std::chrono::steady_clock::now();
		const float timeFromStart = std::chrono::duration<float>(std::chrono::steady_clock::now() - start).count();
		graphics.Draw((std::sin(timeFromStart) + 1.f) / 2);
	}

	return 0;
}