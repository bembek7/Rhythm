#pragma once
#include "Window.h"

class App
{
public:
	int Run();

private:
	enum HitType
	{
		Perfect,
		Good,
		Miss
	};
	HitType EvaluateHit(const double timeFromStart, const float timeStep) noexcept;

private:
	Window window;
};
