#include "App.h"
#include <chrono>
#include "Mesh.h"
#include "PointLight.h"
#include <numbers>
#include "SoundPlayer.h"
#include "Camera.h"

int App::Run()
{
	struct Song
	{
		std::string fileName;
		unsigned int bpm;
	} mainSong;

	mainSong.fileName = "100bpmTest.wav";
	mainSong.bpm = 100;
	const float beatsPerSecond = mainSong.bpm / 60.f;
	const float timeStep = 1 / beatsPerSecond;

	SoundPlayer soundPlayer;

	soundPlayer.LoadSong(mainSong.fileName);

	Mesh plane = Mesh(window.GetGraphics(), "plane.obj", ShaderType::Phong, {0.f, 0.f, 7.f}, {0.f, 0.f, 0.f}, {10.f, 10.f, 1.f});
	{
		Mesh sphere1 = Mesh(window.GetGraphics(), "sphere.obj", ShaderType::Phong, { 5.f, 5.f, 6.f }, { 0.f, 0.f, 0.f }, { 0.5f, 0.5f, 0.5f });
	}
	Mesh sphere = Mesh(window.GetGraphics(), "sphere.obj", ShaderType::Phong, { 0.f, 0.f, 6.f }, { 0.f, 0.f, 0.f }, { 0.5f, 0.5f, 0.5f });
	
	Mesh sphere2 = Mesh(window.GetGraphics(), "sphere.obj", ShaderType::Phong, { 2.f, 2.f, 6.f }, { 0.f, 0.f, 0.f }, { 0.5f, 0.5f, 0.5f });
	Mesh sphere3 = Mesh(window.GetGraphics(), "sphere.obj", ShaderType::Phong, { -3.f, -3.f, 6.f }, { 0.f, 0.f, 0.f }, { 0.5f, 0.5f, 0.5f });
	PointLight planeLight = PointLight(window.GetGraphics(), sphere.GetPosition());
	PointLight sphereLight = PointLight(window.GetGraphics(), { 0.f, 0.f, 0.f });
	Camera camera;
	window.GetGraphics().SetCamera(camera.GetMatrix()); // Should be set every frame if doing camera movement

	soundPlayer.Play(true);
	auto start = std::chrono::steady_clock::now();
	float sphereGreenParameter = 0.5f;
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}

		const float timeFromStart = std::chrono::duration<float>(std::chrono::steady_clock::now() - start).count();
		double currentPlaytime = (timeFromStart + soundPlayer.GetCurrentPlaytime()) / 2; // Get current playtime has low granularity

		while (!window.IsKeyQueueEmpty())
		{
			const short keyPressed = window.PopPressedKey();
			if (keyPressed == VK_LBUTTON)
			{
				HitType hitType = EvaluateHit(currentPlaytime, timeStep);
				switch (hitType)
				{
				case HitType::Perfect:
					sphereGreenParameter += 0.1f;
					break;
				case HitType::Good:
					sphereGreenParameter += 0.05f;
					break;
				case HitType::Miss:
					sphereGreenParameter -= 0.15f;
					break;
				}
			}
		}

		float timeColorFactor = float(std::cos(currentPlaytime * 2 * std::numbers::pi / timeStep) + 1.f) / 2;
		timeColorFactor = timeColorFactor * timeColorFactor;

		sphereGreenParameter = std::clamp(sphereGreenParameter, 0.f, 1.f);
		const float sphereRedParameter = 1.f - sphereGreenParameter;
		const DirectX::XMFLOAT4 newColor = { timeColorFactor * sphereRedParameter, timeColorFactor * sphereGreenParameter, 0.0f, 1.0f };

		sphere.SetColor(window.GetGraphics(), newColor);
		planeLight.SetDiffuseColor(window.GetGraphics(), sphere.GetColor());

		window.GetGraphics().BeginFrame();
		sphereLight.Bind(window.GetGraphics());
		sphere.Draw(window.GetGraphics());
		sphere2.Draw(window.GetGraphics());
		sphere3.Draw(window.GetGraphics());
		planeLight.Bind(window.GetGraphics());
		plane.Draw(window.GetGraphics());
		window.GetGraphics().EndFrame();
	}

	return 0;
}

App::HitType App::EvaluateHit(const double timeFromStart, const float timeStep) noexcept
{
	double missTime = std::fmod(timeFromStart, timeStep);
	if (missTime > timeStep / 2)
	{
		missTime = std::abs(timeStep - missTime);
	}

	if (missTime < 0.1f)
	{
		return HitType::Perfect;
	}
	else if (missTime < 0.15f)
	{
		return HitType::Good;
	}
	return HitType::Miss;
}
