#include "Graphics.h"
#include <stdexcept>
#include <comdef.h>

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

	CheckHR(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&scd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pContext
	));

	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
	CheckHR(pSwapChain->GetBuffer(0u, __uuidof(ID3D11Resource), &pBackBuffer));
	CheckHR(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pRenderTargetView));

	pContext->OMSetRenderTargets(1u, pRenderTargetView.GetAddressOf(), nullptr);

	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = windowWidth;
	viewport.Height = windowHeight;
	viewport.MaxDepth = 1.f;
	viewport.MinDepth = 0.f;

	pContext->RSSetViewports(1u, &viewport);
}

void Graphics::Draw()
{
	const float color[] = { 1.f, 0.f, 0.f, 1.0f};
	pContext->ClearRenderTargetView(pRenderTargetView.Get(), color);
	CheckHR(pSwapChain->Present(1u, 0u));
}

void Graphics::CheckHR(HRESULT hr)
{
	if (FAILED(hr)) {
		_com_error err = _com_error(hr);
		MessageBoxA(nullptr, err.ErrorMessage(), nullptr, MB_ICONERROR | MB_OK);
		throw std::runtime_error("HRESULT failed");
	}
}
