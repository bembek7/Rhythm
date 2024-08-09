#pragma once
#include "Window.h"

class App
{
public:
	int Run();

private:
	enum class HitType
	{
		Perfect,
		Good,
		Miss
	};
	HitType EvaluateHit(const double timeFromStart, const float timeStep) noexcept;

private:
	Window window = Window(1600, 900);
};
