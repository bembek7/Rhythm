#pragma once
#include "Graphics.h"
#include <string>
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>

enum ShaderType
{
	Solid,
	Phong
};

class Mesh
{
public:
	Mesh(Graphics& graphics, std::string fileName, ShaderType shaderType, DirectX::XMVECTOR position = { 0.f, 0.f, 5.f }, DirectX::XMVECTOR rotation = { 0.f, 0.f, 0.f }, DirectX::XMVECTOR scale = { 1.f, 1.f, 1.f });

	void Draw(Graphics& graphics);

	void AddRotation(const DirectX::XMVECTOR& rotationToAdd) noexcept;
	void AddPosition(const DirectX::XMVECTOR& posiationToAdd) noexcept;
	void Scale(float scaleFactor) noexcept;
	void SetColor(Graphics& graphics, const DirectX::XMFLOAT4& newColor);

private:
	DirectX::XMMATRIX GetTransformMatrix() const noexcept;
	void LoadModel(std::string fileName);
	void UpdateTransformBuffer(Graphics& graphics);
private:
	struct Vertex {
		Vertex(float x, float y, float z, float nx, float ny, float nz) : position(x, y, z), normal(nx, ny, nz) {}
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
	};
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantTransformBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantColorBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	DirectX::XMVECTOR position = { 0.f, 0.f, 5.f };
	DirectX::XMVECTOR rotation = { 0.f, 0.f, 0.f };
	DirectX::XMVECTOR scale = { 1.f, 1.f, 1.f };

	struct ConstantTransformBuffer
	{
		ConstantTransformBuffer() = default;
		ConstantTransformBuffer(const DirectX::XMMATRIX& newTransformView, const DirectX::XMMATRIX& newTransformViewProjection) :
			transformView(newTransformView),
			transformViewProjection(newTransformViewProjection)
			{}
		DirectX::XMMATRIX transformView;
		DirectX::XMMATRIX transformViewProjection;
	};
	ConstantTransformBuffer transformBuffer;

	struct ConstantColorBuffer
	{
		ConstantColorBuffer() = default;
		ConstantColorBuffer(const DirectX::XMFLOAT4 newColor) :
			color(newColor)
		{}
		DirectX::XMFLOAT4 color;
	};
	ConstantColorBuffer colorBuffer = { { 1.0f, 1.0f, 1.0f, 1.0f } };
};
