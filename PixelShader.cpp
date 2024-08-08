#include "PixelShader.h"
#include <string>
#include "ThrowMacros.h"
#include <d3dcompiler.h>

PixelShader::PixelShader(Graphics& graphics, std::wstring shaderPath)
{
	CHECK_HR(D3DReadFileToBlob(shaderPath.c_str(), &blob));
	CHECK_HR(GetDevice(graphics)->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader));
}

void PixelShader::Bind(Graphics& graphics) noexcept
{
	GetContext(graphics)->PSSetShader(pixelShader.Get(), nullptr, 0u);
}