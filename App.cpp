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

	int mouseCounter = 0;

	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}

		const float deltaTime = std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
		last = std::chrono::steady_clock::now();


		while (!window.IsKeyQueueEmpty())
		{
			const short keyPressed = window.PopPressedKey();
			if (keyPressed == VK_LBUTTON)
			{
				mouseCounter++;
				std::stringstream ss;
				ss << mouseCounter;
				window.SetWidnowTitle(ss.str());

			}
		}

		if (window.IsKeyPressed('W'))
		{
			box.AddPosition({ 0.f,0.f,0.1f });
		}
		if (window.IsKeyPressed('S'))
		{
			box.AddPosition({ 0.f,0.f,-0.1f });
		}
		if (window.IsKeyPressed('Q'))
		{
			box.AddRotation({ 0.f,0.f,0.1f });
		}
		if (window.IsKeyPressed('E'))
		{
			box.AddRotation({ 0.f,0.f,-0.1f });
		}
		if (window.IsKeyPressed(VK_RIGHT))
		{
			box.AddRotation({ 0.f,0.1f,0.f });
		}
		if (window.IsKeyPressed(VK_LEFT))
		{
			box.AddRotation({ 0.f,-0.1f,0.f });
		}
		if (window.IsKeyPressed(VK_UP))
		{
			box.AddRotation({ 0.1f,0.f,0.f });
		}
		if (window.IsKeyPressed(VK_DOWN))
		{
			box.AddRotation({ -0.1f,0.f,0.f });
		}
		
		graphics.BeginFrame();
		box.Draw(graphics);
		graphics.EndFrame();
	}

	//gSoloud.deinit(); // Clean up!
	return 0;
}