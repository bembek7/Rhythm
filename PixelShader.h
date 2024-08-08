#pragma once
#include "Bindable.h"
#include <string>

class PixelShader : public Bindable
{
public:
	PixelShader(Graphics& graphics, std::wstring shaderPath);
	void Bind(Graphics& graphics) noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
};