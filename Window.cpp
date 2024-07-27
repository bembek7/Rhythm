#include "Window.h"
#include <system_error>
#include <optional>
#include <cassert>

Window::Window(HINSTANCE hInstance)
{
	WNDCLASSEX windowClass = {};
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = 0u;
	windowClass.lpfnWndProc = Window::WindowProcBeforeCreation;
	windowClass.lpszClassName = winClassName;

	RegisterClassEx(&windowClass);

	hWnd = CreateWindowEx(
		0,
		winClassName,
		"Rythm",
		WS_OVERLAPPEDWINDOW,
		100,
		100,
		windowWidth,
		windowHeight,
		nullptr,
		nullptr,
		hInstance,
		this
	);

	if (!hWnd)
	{
		MessageBox(nullptr, std::system_category().message(GetLastError()).c_str(), nullptr, MB_ICONERROR | MB_OK);
	}

	ShowWindow(hWnd, SW_SHOW);
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg = { };
	while (PeekMessage(&msg, NULL, 0u, 0u, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return (int)msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}

HWND& Window::GetHWNDRef() noexcept
{
	return hWnd;
}

unsigned int Window::GetWindowWidth() const noexcept
{
	return windowWidth;
}

unsigned int Window::GetWindowHeight() const noexcept
{
	return windowHeight;
}

LRESULT Window::WindowProcBeforeCreation(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		const CREATESTRUCTW* const csw = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const window = static_cast<Window*>(csw->lpCreateParams);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::WindowProcAfterCreation));

		assert(window != nullptr);
		return window->HandleMessage(hWnd, uMsg, wParam, lParam);
	}
	
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::WindowProcAfterCreation(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window* const window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	return window->HandleMessage(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		keys[wParam] = true;
		break;
	case WM_KEYUP:
		keys[wParam] = false;
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
