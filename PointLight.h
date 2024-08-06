#pragma once
#include <DirectXMath.h>
#include "Graphics.h"

class PointLight
{
public:
	PointLight(Graphics& graphics, const DirectX::XMFLOAT3& position);
	void SetDiffuseColor(Graphics& graphics, const DirectX::XMFLOAT3& newColor);
	void Bind(Graphics& graphics);

private:
	void UpdateBuffer(Graphics& graphics);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantLightBuffer;

	struct ConstantLightBuffer
	{
		DirectX::XMFLOAT3 diffuseColor = { 1.0f, 1.0f, 1.0f };
		float diffuseIntensity = 0.4f;
		DirectX::XMFLOAT3 ambient = { 0.3f, 0.3f, 0.3f };
		float specularIntensity = 0.7f;
		DirectX::XMFLOAT3 lightPosition = { 0.f, 0.f, 0.f };
		float specularPower = 0.8f;
	} lightBuffer;
};
