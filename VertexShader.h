#pragma once
#include "Bindable.h"
#include <string>

class VertexShader : public Bindable
{
public:
	VertexShader(Graphics& graphics, std::wstring shaderPath);
	void Bind(Graphics& graphics) noexcept;
	ID3DBlob* GetBlob() noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
};