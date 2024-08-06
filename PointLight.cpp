#include "PointLight.h"
#include "ThrowMacros.h"

PointLight::PointLight(Graphics& graphics, const DirectX::XMFLOAT3& position)
{
	lightBuffer.lightPosition = position;

	D3D11_BUFFER_DESC lightBufferDesc{};
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.ByteWidth = sizeof(lightBuffer);

	D3D11_SUBRESOURCE_DATA lightBufferData = {};
	lightBufferData.pSysMem = &lightBuffer;

	CHECK_HR(graphics.device->CreateBuffer(&lightBufferDesc, &lightBufferData, &constantLightBuffer));
}

void PointLight::SetDiffuseColor(Graphics& graphics, const DirectX::XMFLOAT3& newColor)
{
	lightBuffer.diffuseColor = newColor;
	UpdateBuffer(graphics);
}

void PointLight::Bind(Graphics& graphics)
{
	graphics.context->PSSetConstantBuffers(1u, 1u, constantLightBuffer.GetAddressOf());
}

void PointLight::UpdateBuffer(Graphics& graphics)
{
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	CHECK_HR(graphics.context->Map(constantLightBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubresource));
	memcpy(mappedSubresource.pData, &lightBuffer, sizeof(lightBuffer));
	graphics.context->Unmap(constantLightBuffer.Get(), 0u);
}