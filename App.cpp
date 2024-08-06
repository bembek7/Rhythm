#include "App.h"
#include "Graphics.h"
#include "SoLoud/include/soloud.h"
#include "SoLoud/include/soloud_wav.h"
#include <chrono>
#include "Mesh.h"
#include "PointLight.h"
#include <numbers>

int App::Run()
{
	Graphics graphics = Graphics(window.GetHWNDRef(), window.GetWindowWidth(), window.GetWindowHeight());

	auto last = std::chrono::steady_clock::now();
	auto start = std::chrono::steady_clock::now();

	constexpr float beat = 1.0f;

	struct Song
	{
		std::string fileName;
		unsigned int bpm;
	} testSong;

	testSong.fileName = "120bpmTest.wav";
	testSong.bpm = 120;

	SoLoud::Soloud gSoloud; // SoLoud engine
	SoLoud::Wav gWave;      // One wave file
	gSoloud.init(); // Initialize SoLoud
	unsigned int x = gWave.load(testSong.fileName.c_str()); // Load a wave
	if (x > 0)
	{
		throw std::runtime_error(gSoloud.getErrorString(x));
	}

	gWave.setLooping(true);
	gSoloud.play(gWave);

	Mesh plane = Mesh(graphics, "plane.obj", Phong, { 0.f, 0.f, 7.f }, { 0.f, 0.f, 0.f }, { 10.f, 10.f, 1.f });
	Mesh sphere = Mesh(graphics, "sphere.obj", Phong, { 0.f, 0.f, 6.f }, { 0.f, 0.f, 0.f }, { 0.5f, 0.5f, 0.5f });
	PointLight planeLight = PointLight(graphics, { 0.f, 0.f, 6.f });
	PointLight sphereLight = PointLight(graphics, { 0.f, 0.f, 0.f });

	Camera camera;

	float screenRatio = static_cast<float>(window.GetWindowHeight()) / window.GetWindowWidth();
	graphics.SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, screenRatio, 0.7f, 50.0f));

	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}

		const float deltaTime = std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
		last = std::chrono::steady_clock::now();

		const float timeFromStart = std::chrono::duration<float>(std::chrono::steady_clock::now() - start).count();
		const float beatsPerSecond = testSong.bpm / 60.f;
		const float timeStep = 1 / beatsPerSecond;
		float redFactor = (std::cos(timeFromStart * 2 * std::numbers::pi / timeStep) + 1.f) / 2;
		redFactor = redFactor * redFactor;
		const DirectX::XMFLOAT4 newColor = { redFactor, 0.0f, 0.0f, 1.0f };
		sphere.SetColor(graphics, newColor);
		planeLight.SetDiffuseColor(graphics, DirectX::XMFLOAT3{ newColor.x, newColor.y, newColor.z });

		while (!window.IsKeyQueueEmpty())
		{
			const short keyPressed = window.PopPressedKey();
			if (keyPressed == VK_LBUTTON)
			{
				
			}
		}
		float timeOffset = 0.f;
		timeOffset = std::fmod(timeFromStart, timeStep);
		if (timeOffset > timeStep / 2)
		{
			timeOffset = timeStep - timeOffset;
		}
		std::stringstream ss;
		ss << timeOffset;
		window.SetWidnowTitle(ss.str());
		if (window.IsKeyPressed('W'))
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
		}

		graphics.SetCamera(camera.GetMatrix());
		graphics.BeginFrame();
		sphereLight.Bind(graphics);
		sphere.Draw(graphics);
		planeLight.Bind(graphics);
		plane.Draw(graphics);
		graphics.EndFrame();
	}

	//gSoloud.deinit(); // Clean up!
	return 0;
}