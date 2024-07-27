#pragma once
#include<Windows.h>
#include <optional>

class Window
{
public:
	Window(HINSTANCE hInstance);
	~Window() = default;
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	static std::optional<int> ProcessMessages();
	HWND& GetHWNDRef() noexcept;
	unsigned int GetWindowWidth() const noexcept;
	unsigned int GetWindowHeight() const noexcept;

private:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
private:
	HWND hWnd;
	static constexpr char winClassName[] = "RythmWindow";
	const unsigned int windowWidth = 800;
	const unsigned int windowHeight = 600;
};

