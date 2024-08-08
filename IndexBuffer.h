#pragma once
#include "ThrowMacros.h"
#include "Bindable.h"
#include <vector>

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics& graphics, const std::vector<unsigned int>& indices);
	void Bind(Graphics& graphics) noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
};
