#include "Windows.h"
#include <system_error>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR pCmdLine, int nCmdShow)
{
	const char winClassName[] = "RythmWindow";

	WNDCLASSEXA windowClass = {};
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = 0u;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.lpszClassName = winClassName;

	RegisterClassExA(&windowClass);

	HWND hWnd = CreateWindowExA(
		0,
		winClassName,
		"Rythm",
		WS_OVERLAPPEDWINDOW,
		100,
		100,
		800,
		640,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	if (!hWnd)
	{
		MessageBoxA(nullptr, std::system_category().message(GetLastError()).c_str(), nullptr, MB_ICONERROR | MB_OK);
	}

	ShowWindow(hWnd, nCmdShow);

	MSG msg = { };
	while (true)
	{
		while (PeekMessageA(&msg, NULL, 0, 0, true))
		{
			if (msg.message == WM_QUIT)
			{
				return (int)msg.wParam;
			}

			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		// Graphics here
	}
	
	return 0;
}