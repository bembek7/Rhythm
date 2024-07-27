#include "Window.h"
#include <system_error>
#include <optional>

Window::Window(HINSTANCE hInstance)
{
	WNDCLASSEX windowClass = {};
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = 0u;
	windowClass.lpfnWndProc = Window::WindowProc;
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
		nullptr
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

LRESULT Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		if (wParam == 'K')
		{
			SetWindowText(hwnd, "Key down");
		}
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
