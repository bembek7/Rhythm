#pragma once
#include "BetterWindows.h"
#include <dxgi.h>
#include <d3d11.h>
#include <wrl\client.h>

class Graphics
{
	friend class Mesh;
public:
	Graphics(HWND& hWnd, unsigned int windowWidth, unsigned int windowHeight);
	~Graphics() = default;
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	void BeginFrame() noexcept;
	void EndFrame();

private:
	void DrawIndexed(const size_t numIndices);

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
};
