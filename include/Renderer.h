#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <dxgi1_3.h>
#include <winrt/base.h>

using winrt::com_ptr;

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void updateLoop(float deltaTime);
    void drawFrame();
    void initialize(HWND hwnd);
protected:
    HWND mHwnd;

    com_ptr<ID3D11Device> device;
    com_ptr<ID3D11DeviceContext> context;
    com_ptr<IDXGISwapChain> swapChain;
    com_ptr<ID3D11Texture2D> backBuffer;

    com_ptr<ID3D11PixelShader> pxlShader;
    com_ptr<ID3D11VertexShader> vtxShader;

    com_ptr<ID3D11InputLayout> inputLayout;

    com_ptr<ID3D11Buffer> vrtBuffer;
    com_ptr<ID3D11Buffer> idxBuffer;

    D3D11_TEXTURE2D_DESC backBufferDesc;

    com_ptr<ID3D11RenderTargetView> renderTarget;

    D3D11_VIEWPORT viewPort;


    FLOAT redval;
    FLOAT totalTime;

    void loadShader();
};