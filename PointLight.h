#pragma once
#include <DirectXMath.h>
#include "Graphics.h"
#include "ConstantBuffer.h"

class PointLight
{
public:
	PointLight(Graphics& graphics, const DirectX::XMFLOAT3& position);
	void SetDiffuseColor(Graphics& graphics, const DirectX::XMFLOAT3& newColor);
	void Bind(Graphics& graphics);

private:
	struct LightBuffer
	{
		DirectX::XMFLOAT3 diffuseColor = { 1.0f, 1.0f, 1.0f };
		float diffuseIntensity = 0.6f;
		DirectX::XMFLOAT3 ambient = { 0.35f, 0.35f, 0.35f };
		float specularIntensity = 0.6f;
		DirectX::XMFLOAT3 lightPosition = { 0.f, 0.f, 0.f };
		float specularPower = 0.8f;
	} lightBuffer;

	std::unique_ptr<ConstantBuffer<LightBuffer>> constantLightBuffer;
};
