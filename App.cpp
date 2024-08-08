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

	mainSong.fileName = "120bpmTest.wav";
	mainSong.bpm = 120;
	const float beatsPerSecond = mainSong.bpm / 60.f;
	const float timeStep = 1 / beatsPerSecond;

	SoundPlayer soundPlayer;

	soundPlayer.LoadSong(mainSong.fileName);

	Mesh plane = Mesh(window.GetGraphics(), "plane.obj", Phong, {0.f, 0.f, 7.f}, {0.f, 0.f, 0.f}, {10.f, 10.f, 1.f});
	Mesh sphere = Mesh(window.GetGraphics(), "sphere.obj", Phong, { 0.f, 0.f, 6.f }, { 0.f, 0.f, 0.f }, { 0.5f, 0.5f, 0.5f });
	PointLight planeLight = PointLight(window.GetGraphics(), sphere.GetPosition());
	PointLight sphereLight = PointLight(window.GetGraphics(), { 0.f, 0.f, 0.f });
	Camera camera;
	window.GetGraphics().SetCamera(camera.GetMatrix()); // Should be set every frame if doing camera movement

	auto start = std::chrono::steady_clock::now();
	soundPlayer.Play(true);

	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}

		const float timeFromStart = std::chrono::duration<float>(std::chrono::steady_clock::now() - start).count();
		float blueFactor = float(std::cos(timeFromStart * 2 * std::numbers::pi / timeStep) + 1.f) / 2;
		blueFactor = blueFactor * blueFactor;
		const DirectX::XMFLOAT4 newColor = { 0.0f, blueFactor / 1.5f, blueFactor, 1.0f };
		sphere.SetColor(window.GetGraphics(), newColor);
		planeLight.SetDiffuseColor(window.GetGraphics(), sphere.GetColor());

		while (!window.IsKeyQueueEmpty())
		{
			const short keyPressed = window.PopPressedKey();
			if (keyPressed == VK_LBUTTON)
			{
				EvaluateHit(timeFromStart, timeStep);
			}
		}

		/*if (window.IsKeyPressed('W'))
		{
			plane.AddPosition({ 0.f,0.f,0.1f });
		}
		if (window.IsKeyPressed('S'))
		{
			plane.AddPosition({ 0.f,0.f,-0.1f });
		}
		if (window.IsKeyPressed('Q'))
		{
			plane.AddRotation({ 0.f,0.f,0.1f });
		}
		if (window.IsKeyPressed('E'))
		{
			plane.AddRotation({ 0.f,0.f,-0.1f });
		}
		if (window.IsKeyPressed(VK_RIGHT))
		{
			plane.AddRotation({ 0.f,0.1f,0.f });
		}
		if (window.IsKeyPressed(VK_LEFT))
		{
			plane.AddRotation({ 0.f,-0.1f,0.f });
		}
		if (window.IsKeyPressed(VK_UP))
		{
			plane.AddRotation({ 0.1f,0.f,0.f });
		}
		if (window.IsKeyPressed(VK_DOWN))
		{
			plane.AddRotation({ -0.1f,0.f,0.f });
		}
		if (window.IsKeyPressed('A'))
		{
			plane.Scale(0.9f);
		}
		if (window.IsKeyPressed('D'))
		{
			plane.Scale(1.1f);
		}*/

		window.GetGraphics().BeginFrame();
		sphereLight.Bind(window.GetGraphics());
		sphere.Draw(window.GetGraphics());
		planeLight.Bind(window.GetGraphics());
		plane.Draw(window.GetGraphics());
		window.GetGraphics().EndFrame();
	}

	return 0;
}

void App::EvaluateHit(const float timeFromStart, const float timeStep) noexcept
{
	float missTime = std::fmod(timeFromStart, timeStep);
	if (missTime > timeStep / 2)
	{
		missTime = std::abs(timeStep - missTime);
	}

	if (missTime < 0.075f)
	{
		PerfectHit();
	}
	else if (missTime < 0.15f)
	{
		GoodHit();
	}
	else
	{
		MissedHit();
	}
}

void App::PerfectHit() noexcept
{
}

void App::GoodHit() noexcept
{
}

void App::MissedHit() noexcept
{
}