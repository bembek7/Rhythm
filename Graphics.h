#pragma once
#include <Windows.h>
#include <wrl.h>
#include <dxgi.h>
#include <d3d11.h>

class Graphics
{
public:
	Graphics(HWND& hWnd, int windowWidth, int windowHeight);
	~Graphics() = default;
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	void Draw(const float red);

private:
	void CheckHR(HRESULT hr);
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
};

