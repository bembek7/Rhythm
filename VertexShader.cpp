#include "VertexShader.h"
#include <d3dcompiler.h>
#include <string>
#include "ThrowMacros.h"
#include "Graphics.h"

VertexShader::VertexShader(Graphics& graphics, const std::wstring& shaderPath)
{
	CHECK_HR(D3DReadFileToBlob(shaderPath.c_str(), &blob));
	CHECK_HR(GetDevice(graphics)->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader));
}

void VertexShader::Bind(Graphics& graphics) noexcept
{
	GetContext(graphics)->VSSetShader(vertexShader.Get(), nullptr, 0u);
}

LPVOID VertexShader::GetBufferPointer() noexcept
{
	return blob->GetBufferPointer();
}

size_t VertexShader::GetBufferSize() const noexcept
{
	return blob->GetBufferSize();
}

