#pragma once
#include "Graphics.h"
#include <string>
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>

class Mesh
{
public:
	Mesh(Graphics& graphics, std::string fileName);

	void Draw(Graphics& graphics);

	void AddRotation(const DirectX::XMVECTOR& rotationToAdd) noexcept;
	void AddPosition(const DirectX::XMVECTOR& posiationToAdd) noexcept;

private:
	DirectX::XMMATRIX GetTransformMatrix() const noexcept;
	void LoadModel(std::string fileName);
	void UpdateTransformBuffer(Graphics& graphics);
private:
	struct Vertex {
		Vertex(float x, float y, float z) : pos(x, y, z) {}
		DirectX::XMFLOAT3 pos;
	};
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantTransformBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	DirectX::XMVECTOR position = { 0.f, 0.f, 3.f };
	DirectX::XMVECTOR rotation = { 0.f, 0.f, 0.f };

	struct ConstantBuffer
	{
		DirectX::XMMATRIX transform;
	};
	ConstantBuffer transformBuffer =
	{
		{
			DirectX::XMMatrixTranspose(
				DirectX::XMMatrixRotationRollPitchYawFromVector(rotation) *
				DirectX::XMMatrixTranslationFromVector(position) *
				DirectX::XMMatrixPerspectiveLH(1.0f,3.0f / 4.0f,0.5f,10.0f)
			)
		}
	};
};
