#include "Graphics.h"
#include <stdexcept>
#include <comdef.h>
#include <filesystem>
#include <d3dcompiler.h>
#include <DirectXMath.h>

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

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Graphics::Draw(const float red)
{
	const float color[] = { red, 0.f, 0.f, 1.0f};
	context->ClearRenderTargetView(renderTargetView.Get(), color);
	DrawTriangle();
	CHECK_HR(swapChain->Present(1u, 0u));
}

void Graphics::DrawTriangle()
{
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	CHECK_HR(D3DReadFileToBlob(L"PixelShader.cso", &blob));
	CHECK_HR(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader));
	context->PSSetShader(pixelShader.Get(), nullptr, 0u);

	const unsigned int indices[] =
	{
		0, 1, 2
	};
	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0u;
	indexBufferDesc.MiscFlags = 0u;
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.StructureByteStride = sizeof(unsigned int);

	D3D11_SUBRESOURCE_DATA indexBufferData = {};
	indexBufferData.pSysMem = indices;

	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	CHECK_HR(device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer));
	context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);

	struct Vertex {
		Vertex(float x, float y, float z, float w) : pos(x, y, z, w) {}
		DirectX::XMFLOAT4 pos;
	};
	const Vertex vertices[] =
	{
		{ 0.0f,0.7f, 1.f, 1.f},
		{ 0.7f,-0.7f, 1.f, 1.f},
		{ -0.7f,-0.7f, 1.f, 1.f}
	};
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.CPUAccessFlags = 0u;
	vertexBufferDesc.MiscFlags = 0u;
	vertexBufferDesc.ByteWidth = sizeof(vertices);
	vertexBufferDesc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA vertexBufferData = {};
	vertexBufferData.pSysMem = vertices;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	CHECK_HR(device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer));
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	context->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	CHECK_HR(D3DReadFileToBlob(L"VertexShader.cso", &blob));
	CHECK_HR(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader));
	context->VSSetShader(vertexShader.Get(), nullptr, 0u);

	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] =
	{
		{"Position", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u},
	};
	CHECK_HR(device->CreateInputLayout(inputLayoutDesc, (UINT)std::size(inputLayoutDesc), blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout));

	context->IASetInputLayout(inputLayout.Get());

	context->DrawIndexed(3u, 0u, 0);
}
