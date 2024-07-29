#include "Graphics.h"
#include <stdexcept>
#include <comdef.h>
#include <filesystem>


#define CHECK_HR(hr) \
    if (FAILED(hr)) { \
        _com_error err(hr); \
        const std::filesystem::path filePath(__FILE__); \
        std::stringstream ss; \
        ss << err.ErrorMessage() << std::endl << filePath.filename() << " " << __LINE__; \
        throw std::runtime_error(ss.str()); \
    }

Graphics::Graphics(HWND& hWnd, int windowWidth, int windowHeight)
{
	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferDesc.Width = 0;
	scd.BufferDesc.Height = 0;
	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 0;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hWnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = 0;

	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	CHECK_HR(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&scd,
		&swapChain,
		&device,
		nullptr,
		&context
	));

	Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
	CHECK_HR(swapChain->GetBuffer(0u, __uuidof(ID3D11Resource), &backBuffer));
	CHECK_HR(device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView));

	context->OMSetRenderTargets(1u, renderTargetView.GetAddressOf(), nullptr);

	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = windowWidth;
	viewport.Height = windowHeight;
	viewport.MaxDepth = 1.f;
	viewport.MinDepth = 0.f;

	context->RSSetViewports(1u, &viewport);
}

void Graphics::Draw(const float red)
{
	const float color[] = { red, 0.f, 0.f, 1.0f};
	context->ClearRenderTargetView(renderTargetView.Get(), color);
	CHECK_HR(swapChain->Present(1u, 0u));
}