#pragma once
#include <vector>
#include <string>
#include <d3d11.h>
#include <winrt/base.h>

namespace Shaders
{
    winrt::com_ptr<ID3D11VertexShader> createVertexShader(winrt::com_ptr<ID3D11Device> const& device, std::string const& fileName);
    winrt::com_ptr<ID3D11PixelShader> createPixelShader(winrt::com_ptr<ID3D11Device> const& device, std::string const& fileName);

    winrt::com_ptr<ID3D11VertexShader> createVertexShaderAndInputLayout(
        winrt::com_ptr<ID3D11Device> const& device, std::string const& fileName, D3D11_INPUT_ELEMENT_DESC const layoutDesc[], size_t layoutSize,
        _Out_ winrt::com_ptr<ID3D11InputLayout>& layout);
}