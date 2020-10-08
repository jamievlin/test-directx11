#include "common.h"
#include "Shaders.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include <d3d11.h>
#include <winrt/base.h>

using common::throwIfFailed;

namespace Shaders
{
    std::vector<uint8_t> readBytecode(std::string const& fileName);


    std::vector<uint8_t> readBytecode(std::string const& fileName)
    {
        std::ifstream file;
        file.open(fileName, std::ifstream::in | std::ifstream::binary);
        std::stringstream byteSrc;

        if (file)
        {
            byteSrc << file.rdbuf();
            file.close();
        }
        else
        {
            throw 1;
        }

        std::vector<uint8_t> outchr;
        std::string const& outchrTmp = byteSrc.str();
        outchr.reserve(outchrTmp.size());
        outchr.assign(outchrTmp.begin(), outchrTmp.end());

        return outchr;
    }

    winrt::com_ptr<ID3D11VertexShader> createVertexShader(winrt::com_ptr<ID3D11Device> const& device, std::string const& fileName)
    {
        winrt::com_ptr<ID3D11VertexShader> outShader;
        auto byteCode = readBytecode(fileName);
        throwIfFailed(device->CreateVertexShader(byteCode.data(), byteCode.size(), nullptr, outShader.put()));
        return outShader;
    }

    winrt::com_ptr<ID3D11VertexShader> createVertexShaderAndInputLayout(
        winrt::com_ptr<ID3D11Device> const& device, std::string const& fileName, D3D11_INPUT_ELEMENT_DESC const layoutDesc[], size_t layoutSize,
        _Out_ winrt::com_ptr<ID3D11InputLayout>& layout)
    {
        winrt::com_ptr<ID3D11VertexShader> outShader;
        auto byteCode = readBytecode(fileName);
        throwIfFailed(device->CreateVertexShader(byteCode.data(), byteCode.size(), nullptr, outShader.put()));
        throwIfFailed(device->CreateInputLayout(layoutDesc, layoutSize, byteCode.data(), byteCode.size(), layout.put()));

        return outShader;
    }

    winrt::com_ptr<ID3D11PixelShader> createPixelShader(winrt::com_ptr<ID3D11Device> const& device, std::string const& fileName)
    {
        winrt::com_ptr<ID3D11PixelShader> outShader;
        auto byteCode = readBytecode(fileName);
        throwIfFailed(device->CreatePixelShader(byteCode.data(), byteCode.size(), nullptr, outShader.put()));
        return outShader;
    }
}