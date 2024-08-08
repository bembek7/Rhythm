#include "App.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR pCmdLine, int nCmdShow)
{
	try
	{
		App app;

		return app.Run();
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), nullptr, MB_ICONERROR | MB_OK);
	}
	return 0;
}