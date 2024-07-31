#include "App.h"
#include "Graphics.h"
//#include "SoLoud/include/soloud.h"
//#include "SoLoud/include/soloud_wav.h"
#include <chrono>
#include "Mesh.h"

int App::Run()
{
	Graphics graphics = Graphics(window.GetHWNDRef(), window.GetWindowWidth(), window.GetWindowHeight());

	auto last = std::chrono::steady_clock::now();

	constexpr float beat = 1.0f;

	//SoLoud::Soloud gSoloud; // SoLoud engine
	//SoLoud::Wav gWave;      // One wave file
	//gSoloud.init(); // Initialize SoLoud
	//unsigned int x = gWave.load("WAVtest.wav"); // Load a wave

	//gSoloud.play(gWave); // Play the wave

	Mesh box = Mesh(graphics, "box.obj");

	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}

		const float deltaTime = std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
		last = std::chrono::steady_clock::now();

		graphics.BeginFrame();
		box.Draw(graphics);
		graphics.EndFrame();
	}

	//gSoloud.deinit(); // Clean up!
	return 0;
}