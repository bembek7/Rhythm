#pragma once
#include "Bindable.h"
#include <vector>

class InputLayout : public Bindable
{
public:
	InputLayout(Graphics& graphics, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDescs, ID3DBlob* shaderBlob);
	virtual void Bind(Graphics& graphics) noexcept override;
private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};
