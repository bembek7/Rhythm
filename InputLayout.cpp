#include "InputLayout.h"
#include "ThrowMacros.h"

InputLayout::InputLayout(Graphics& graphics, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDescs, ID3DBlob* shaderBlob)
{
	CHECK_HR(GetDevice(graphics)->CreateInputLayout(&inputElementDescs.front(), (UINT)inputElementDescs.size(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout));
}

void InputLayout::Bind(Graphics& graphics) noexcept
{
	GetContext(graphics)->IASetInputLayout(inputLayout.Get());
}