#pragma once
#include "Graphics.h"
#include <string>
#include <vector>
#include <DirectXMath.h>
#include <memory>

enum class ShaderType
{
	Solid,
	Phong
};

class Mesh
{
public:
	Mesh(Graphics& graphics, const std::string fileName, const ShaderType shaderType, const DirectX::XMVECTOR& position = { 0.f, 0.f, 5.f }, const DirectX::XMVECTOR& rotation = { 0.f, 0.f, 0.f }, const DirectX::XMVECTOR& scale = { 1.f, 1.f, 1.f });

	void Draw(Graphics& graphics);

	void AddRotation(const DirectX::XMVECTOR& rotationToAdd) noexcept;
	void AddPosition(const DirectX::XMVECTOR& posiationToAdd) noexcept;
	void Scale(const float scaleFactor) noexcept;
	void SetColor(Graphics& graphics, const DirectX::XMFLOAT4& newColor);
	DirectX::XMFLOAT3 GetColor() const noexcept;
	DirectX::XMFLOAT3 GetPosition() const noexcept;

private:
	DirectX::XMMATRIX GetTransformMatrix() const noexcept;
	void LoadModel(const std::string fileName);
	void SetTransformBuffer(Graphics& graphics);

private:
	struct Vertex {
		Vertex(const float x, const float y, const float z, const float nx, const float ny, const float nz) : position(x, y, z), normal(nx, ny, nz) {}
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
	};
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	std::vector<std::unique_ptr<Bindable>> bindables;

	struct TransformBuffer
	{
		TransformBuffer() = default;
		TransformBuffer(const DirectX::XMMATRIX& newTransformView, const DirectX::XMMATRIX& newTransformViewProjection) :
			transformView(newTransformView),
			transformViewProjection(newTransformViewProjection)
		{}
		DirectX::XMMATRIX transformView;
		DirectX::XMMATRIX transformViewProjection;
	};
	TransformBuffer transformBuffer;

	struct ColorBuffer
	{
		ColorBuffer() = default;
		ColorBuffer(const DirectX::XMFLOAT4 newColor) :
			color(newColor)
		{}
		DirectX::XMFLOAT4 color;
	};
	ColorBuffer colorBuffer = { { 1.0f, 1.0f, 1.0f, 1.0f } };

	DirectX::XMVECTOR position = { 0.f, 0.f, 5.f };
	DirectX::XMVECTOR rotation = { 0.f, 0.f, 0.f };
	DirectX::XMVECTOR scale = { 1.f, 1.f, 1.f };
};
