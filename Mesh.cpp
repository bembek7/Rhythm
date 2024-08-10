#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdexcept>
#include <string>
#include "Graphics.h"
#include <cassert>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "ConstantBuffer.h"

Mesh::Mesh(Graphics& graphics, const std::string fileName, const ShaderType shaderType, const DirectX::XMVECTOR& position, const DirectX::XMVECTOR& rotation, const DirectX::XMVECTOR& scale) :
	position(position),
	rotation(rotation),
	scale(scale)
{
	LoadModel(fileName);

	std::wstring pixelShaderPath;
	std::wstring vertexShaderPath;

	switch (shaderType)
	{
	case ShaderType::Solid:
		pixelShaderPath = L"PixelShader.cso";
		vertexShaderPath = L"VertexShader.cso";
		break;
	case ShaderType::Phong:
		pixelShaderPath = L"PhongPS.cso";
		vertexShaderPath = L"PhongVS.cso";
		break;
	}

	bindables.push_back(std::make_unique<ConstantBuffer<ColorBuffer>>(graphics, colorBuffer, BufferType::Pixel));
	bindables.push_back(std::make_unique<PixelShader>(graphics, pixelShaderPath));
	bindables.push_back(std::make_unique<VertexBuffer<Vertex>>(graphics, vertices));
	bindables.push_back(std::make_unique<IndexBuffer>(graphics, indices));
	bindables.push_back(std::make_unique<ConstantBuffer<TransformBuffer>>(graphics, transformBuffer, BufferType::Vertex));
	auto vertexShader = std::make_unique<VertexShader>(graphics, vertexShaderPath);

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescs =
	{
		{"POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"NORMAL", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u},
	};
	bindables.push_back(std::make_unique<InputLayout>(graphics, inputElementDescs, vertexShader->GetBufferPointer(), vertexShader->GetBufferSize()));
	bindables.push_back(std::move(vertexShader));
}

void Mesh::Draw(Graphics& graphics)
{
	SetTransformBuffer(graphics); // in this case dos not have to be updated every frame but wif wee add any movement of camera or object it should be

	for (auto& bindable : bindables)
	{
		bindable->Update(graphics);
		bindable->Bind(graphics);
	}

	graphics.DrawIndexed(indices.size());
}

void Mesh::AddRotation(const DirectX::XMVECTOR& rotationToAdd) noexcept
{
	rotation = DirectX::XMVectorAdd(rotation, rotationToAdd);
}

void Mesh::AddPosition(const DirectX::XMVECTOR& posiationToAdd) noexcept
{
	position = DirectX::XMVectorAdd(position, posiationToAdd);
}

void Mesh::Scale(const float scaleFactor) noexcept
{
	scale = DirectX::XMVectorScale(scale, scaleFactor);
}

void Mesh::SetColor(Graphics& graphics, const DirectX::XMFLOAT4& newColor)
{
	colorBuffer = newColor;
}

DirectX::XMFLOAT3 Mesh::GetColor() const noexcept
{
	return DirectX::XMFLOAT3(colorBuffer.color.x, colorBuffer.color.y, colorBuffer.color.z);
}

DirectX::XMFLOAT3 Mesh::GetPosition() const noexcept
{
	DirectX::XMFLOAT3 retPosition;
	DirectX::XMStoreFloat3(&retPosition, position);
	return retPosition;
}

DirectX::XMMATRIX Mesh::GetTransformMatrix() const noexcept
{
	return DirectX::XMMatrixScalingFromVector(scale) * DirectX::XMMatrixRotationRollPitchYawFromVector(rotation) * DirectX::XMMatrixTranslationFromVector(position);
}

void Mesh::LoadModel(const std::string fileName)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(fileName,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_GenNormals
	);

	if (!scene)
	{
		throw std::runtime_error(importer.GetErrorString());
	}

	const unsigned int numVertices = scene->mMeshes[0]->mNumVertices;

	vertices.reserve(numVertices);

	assert(scene->mMeshes[0]->HasNormals());
	for (size_t i = 0; i < numVertices; i++)
	{
		vertices.push_back(Vertex(scene->mMeshes[0]->mVertices[i].x, scene->mMeshes[0]->mVertices[i].y, scene->mMeshes[0]->mVertices[i].z,
			scene->mMeshes[0]->mNormals[i].x, scene->mMeshes[0]->mNormals[i].y, scene->mMeshes[0]->mNormals[i].z));
	}

	for (size_t i = 0; i < scene->mMeshes[0]->mNumFaces; i++)
	{
		for (size_t j = 0; j < scene->mMeshes[0]->mFaces[i].mNumIndices; j++)
		{
			indices.push_back(scene->mMeshes[0]->mFaces[i].mIndices[j]);
		}
	}

	assert(vertices.size() > 0 && indices.size() > 0);
}

void Mesh::SetTransformBuffer(Graphics& graphics)
{
	DirectX::XMMATRIX transformView = DirectX::XMMatrixTranspose(GetTransformMatrix() * graphics.GetCamera());
	DirectX::XMMATRIX transformViewProjection = DirectX::XMMatrixTranspose(GetTransformMatrix() * graphics.GetCamera() * graphics.GetProjection());
	transformBuffer = TransformBuffer(std::move(transformView), std::move(transformViewProjection));
}