#include "Graphics.h"
#include <stdexcept>
#include <comdef.h>
#include <filesystem>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>     
#include <assimp/postprocess.h> 

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


	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	CHECK_HR(device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState));

	context->OMSetDepthStencilState(depthStencilState.Get(), 1u);


	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = windowWidth;
	depthDesc.Height = windowHeight;
	depthDesc.MipLevels = 1u;
	depthDesc.ArraySize = 1u;
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.SampleDesc.Count = 1u;
	depthDesc.SampleDesc.Quality = 0u;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil;
	CHECK_HR(device->CreateTexture2D(&depthDesc, nullptr, &depthStencil));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0u;

	CHECK_HR(device->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, &depthStencilView));

	context->OMSetRenderTargets(1u, renderTargetView.GetAddressOf(), depthStencilView.Get());

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
	context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	DrawTriangle();
	CHECK_HR(swapChain->Present(1u, 0u));
}

void Graphics::DrawTriangle()
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile("box.obj",
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	// If the import failed, report it
	if (!scene) 
	{
		throw std::runtime_error(importer.GetErrorString());
	}

	const unsigned int numVertices = scene->mMeshes[0]->mNumVertices;

	struct Vertex {
		Vertex(float x, float y, float z) : pos(x, y, z) {}
		DirectX::XMFLOAT3 pos;
	};

	std::vector<Vertex> vertices;
	vertices.reserve(numVertices);

	for (size_t i = 0; i < numVertices; i++)
	{
		vertices.push_back(Vertex(scene->mMeshes[0]->mVertices[i].x, scene->mMeshes[0]->mVertices[i].y, scene->mMeshes[0]->mVertices[i].z));
	}
	
	std::vector<unsigned int> indices;
	for (size_t i = 0; i < scene->mMeshes[0]->mNumFaces; i++)
	{
		for (size_t j = 0; j < scene->mMeshes[0]->mFaces[i].mNumIndices; j++)
		{
			indices.push_back(scene->mMeshes[0]->mFaces[i].mIndices[j]);
		}
	}

	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	CHECK_HR(D3DReadFileToBlob(L"PixelShader.cso", &blob));
	CHECK_HR(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader));
	context->PSSetShader(pixelShader.Get(), nullptr, 0u);

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0u;
	indexBufferDesc.MiscFlags = 0u;
	indexBufferDesc.ByteWidth = (UINT)indices.size() * sizeof(unsigned int);
	indexBufferDesc.StructureByteStride = sizeof(unsigned int);

	D3D11_SUBRESOURCE_DATA indexBufferData = {};
	indexBufferData.pSysMem = indices.data();

	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	CHECK_HR(device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer));
	context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);


	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.CPUAccessFlags = 0u;
	vertexBufferDesc.MiscFlags = 0u;
	vertexBufferDesc.ByteWidth = (UINT)vertices.size() * sizeof(Vertex);
	vertexBufferDesc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA vertexBufferData = {};
	vertexBufferData.pSysMem = vertices.data();

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	CHECK_HR(device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer));
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	context->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);

	struct ConstantBuffer
	{
		DirectX::XMMATRIX transform;
	};
	const ConstantBuffer transformBuffer =
	{
		{
			DirectX::XMMatrixTranspose(
				DirectX::XMMatrixRotationZ(0) *
				DirectX::XMMatrixRotationX(0) *
				DirectX::XMMatrixTranslation(0.0f,0.0f,0.0f + 3.0f) *
				DirectX::XMMatrixPerspectiveLH(1.0f,3.0f / 4.0f,0.5f,10.0f)
			)
		}
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
	D3D11_BUFFER_DESC constantBufferDesc{};
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags = 0u;
	constantBufferDesc.ByteWidth = sizeof(transformBuffer);
	constantBufferDesc.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA constanBufferData = {};
	constanBufferData.pSysMem = &transformBuffer;
	CHECK_HR(device->CreateBuffer(&constantBufferDesc, &constanBufferData, &constantBuffer));

	context->VSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	CHECK_HR(D3DReadFileToBlob(L"VertexShader.cso", &blob));
	CHECK_HR(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader));
	context->VSSetShader(vertexShader.Get(), nullptr, 0u);

	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] =
	{
		{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u},
	};
	CHECK_HR(device->CreateInputLayout(inputLayoutDesc, (UINT)std::size(inputLayoutDesc), blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout));

	context->IASetInputLayout(inputLayout.Get());

	context->DrawIndexed((UINT)indices.size(), 0u, 0);
}
