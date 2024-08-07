#pragma once
#include "Window.h"

class App
{
public:
	int Run();

private:
	void EvaluateHit(const float timeFromStart, const float timeStep) noexcept;
	void PerfectHit() noexcept;
	void GoodHit() noexcept;
	void MissedHit() noexcept;

private:
	Window window;
};
