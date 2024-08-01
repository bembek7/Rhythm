#pragma once
#include "BetterWindows.h"
#include <optional>
#include <string>
#include <queue>


class Window
{
public:
	Window();
	~Window() = default;
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	static std::optional<int> ProcessMessages();
	HWND& GetHWNDRef() noexcept;
	unsigned int GetWindowWidth() const noexcept;
	unsigned int GetWindowHeight() const noexcept;
	void SetWidnowTitle(std::string newTitle) noexcept;
	bool IsKeyQueueEmpty() const noexcept;
	short PopPressedKey() noexcept;
	bool IsKeyPressed(short key) const noexcept;

private:
	static LRESULT CALLBACK WindowProcBeforeCreation(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WindowProcAfterCreation(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	std::queue<short> keyPressedEvents;
	HWND hWnd;
	bool keys[255] = { false };
	static constexpr char winClassName[] = "RythmWindow";
	const unsigned int windowWidth = 800;
	const unsigned int windowHeight = 600;
};
