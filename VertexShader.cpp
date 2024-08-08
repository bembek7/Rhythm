#include "VertexShader.h"
#include <d3dcompiler.h>
#include <string>
#include "ThrowMacros.h"
#include "Graphics.h"

VertexShader::VertexShader(Graphics& graphics, std::wstring shaderPath)
{
	CHECK_HR(D3DReadFileToBlob(shaderPath.c_str(), &blob));
	CHECK_HR(GetDevice(graphics)->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader));
}

void VertexShader::Bind(Graphics& graphics) noexcept
{
	GetContext(graphics)->VSSetShader(vertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetBlob() noexcept
{
	return blob.Get();
}
