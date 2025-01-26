#include "pch.h"
#include "InputLayout.h"
#include "GraphicsThrows.h"

InputLayout::InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderByteCode)
{
	INFOMAN(gfx);

	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(layout.data(), (UINT)std::size(layout), pVertexShaderByteCode->GetBufferPointer(), pVertexShaderByteCode->GetBufferSize(), &pInputLayout));
}

void InputLayout::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
}