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
	/*struct Vertex {
		Vertex(float x, float y, float z, float nx, float ny, float nz) : position(x, y, z), normal(nx, ny, nz) {}
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
	};*/
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
};
