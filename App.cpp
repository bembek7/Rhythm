#include "App.h"
#include "Graphics.h"
#include "SoLoud/include/soloud.h"
#include "SoLoud/include/soloud_wav.h"
#include <chrono>
#include <numbers>

int App::Run()
{
	graphics = std::make_unique<Graphics>(window.GetHWNDRef(), window.GetWindowWidth(), window.GetWindowHeight());

	auto last = std::chrono::steady_clock::now();
	auto start = std::chrono::steady_clock::now();

	constexpr float beat = 1.0f;

	SoLoud::Soloud gSoloud; // SoLoud engine
	SoLoud::Wav gWave;      // One wave file
	gSoloud.init(); // Initialize SoLoud
	unsigned int x = gWave.load("WAVtest.wav"); // Load a wave


	gSoloud.play(gWave); // Play the wave

	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}

		const float deltaTime = std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
		last = std::chrono::steady_clock::now();
		const float timeFromStart = std::chrono::duration<float>(std::chrono::steady_clock::now() - start).count();
		graphics->Draw((std::sin(timeFromStart * 2 * std::numbers::pi / beat) + 1.f) / 2.f);
	}

	gSoloud.deinit(); // Clean up!
	return 0;
}
