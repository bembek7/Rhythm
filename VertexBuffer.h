#pragma once
#include "ThrowMacros.h"
#include "Bindable.h"
#include <vector>

template<typename Vertex>
class VertexBuffer : public Bindable
{
public:
	VertexBuffer(Graphics& graphics, const std::vector<Vertex>& vertices)
	{
		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = (UINT)vertices.size() * sizeof(Vertex);
		vertexBufferDesc.StructureByteStride = sizeof(Vertex);

		D3D11_SUBRESOURCE_DATA vertexBufferData = {};
		vertexBufferData.pSysMem = vertices.data();

		CHECK_HR(GetDevice(graphics)->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer));
	}
	void Bind(Graphics& graphics) noexcept
	{
		const UINT stride = sizeof(Vertex);
		const UINT offset = 0u;
		GetContext(graphics)->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);
	}
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
};