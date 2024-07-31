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
private:
	void LoadModel(std::string fileName);
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
};
