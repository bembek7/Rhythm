#include "PointLight.h"
#include "ThrowMacros.h"

PointLight::PointLight(Graphics& graphics, const DirectX::XMFLOAT3& position)
{
	lightBuffer.lightPosition = position;

	constantLightBuffer = std::make_unique<ConstantBuffer<LightBuffer>>(graphics, lightBuffer, 1u);
}

void PointLight::SetDiffuseColor(Graphics& graphics, const DirectX::XMFLOAT3& newColor)
{
	lightBuffer.diffuseColor = newColor;
	constantLightBuffer->Update(graphics, lightBuffer);
}

void PointLight::Bind(Graphics& graphics)
{
	constantLightBuffer->Bind(graphics);
}
