#include "Renderer.h"        
#include "common.h"
#include "Shaders.h"
#include <vector>
#include <cmath>

#if defined(_DEBUG)
std::string const SHADER_DIR = "x64/Debug/";
#else
std::string const SHADER_DIR = "./"
#endif

using common::throwIfFailed;

Renderer::Renderer() :
    mHwnd(NULL), totalTime(0.f), redval(0.f) {

}

Renderer::~Renderer() {

}

void Renderer::initialize(HWND hwnd) {
    mHwnd = hwnd;
    D3D_FEATURE_LEVEL levels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_1
    };
    UINT devFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
    devFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    D3D_FEATURE_LEVEL chkLvl;
    throwIfFailed(D3D11CreateDevice(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, devFlags, levels, ARRAYSIZE(levels),
        D3D11_SDK_VERSION, device.put(), &chkLvl, context.put()));


    // initialize dx swap chain
    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
    desc.Windowed = TRUE;
    desc.BufferCount = 2;
    desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;

    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    desc.OutputWindow = mHwnd;

    winrt::com_ptr<IDXGIDevice3> dxgi_dev = device.as<IDXGIDevice3>();
    winrt::com_ptr<IDXGIAdapter> adap;
    winrt::com_ptr<IDXGIFactory> fact;
    
    throwIfFailed(dxgi_dev->GetAdapter(adap.put()));
    adap->GetParent(__uuidof(fact), fact.put_void());

    throwIfFailed(fact->CreateSwapChain(device.get(), &desc, swapChain.put()));


    //get render target
    swapChain->ResizeBuffers(2, MainWindowNames::WIN_WIDTH, MainWindowNames::WIN_HEIGHT, DXGI_FORMAT_B8G8R8A8_UNORM, 0);

    throwIfFailed(swapChain->GetBuffer(0, __uuidof(backBuffer), backBuffer.put_void()));
    throwIfFailed(device->CreateRenderTargetView(backBuffer.get(), nullptr, renderTarget.put()));

    backBuffer->GetDesc(&backBufferDesc);

    // set viewport
    ZeroMemory(&viewPort, sizeof(D3D11_VIEWPORT));
    viewPort.Height = (float)backBufferDesc.Height;
    viewPort.Width = (float)backBufferDesc.Width;
    viewPort.MinDepth = 0;
    viewPort.MaxDepth = 1;

    context->RSSetViewports(1, &viewPort);

    loadShader();
}

void Renderer::loadShader() {
    std::string shaderDir;
    D3D11_INPUT_ELEMENT_DESC const basicVertLayoutDesc[] = {
        {"VTX_INPUT", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    Shaders::createVertexShaderAndInputLayout(
        this->device, SHADER_DIR + "/vs.cso", basicVertLayoutDesc, 
        ARRAYSIZE(basicVertLayoutDesc), inputLayout
        ).copy_to(vtxShader.put());
    Shaders::createPixelShader(this->device, SHADER_DIR + "/ps.cso").copy_to(pxlShader.put());
}

void Renderer::updateLoop(float deltaTime) {
    totalTime += deltaTime;
    redval = 0.5 + (0.5 * std::sin(0.001 * totalTime));

    vrtBuffer = nullptr;
    idxBuffer = nullptr;

    std::vector<float> triangleVerts {
        -0.5f, -0.5f,
        0.5f, redval,
        0.5f, -0.5f
    };

    D3D11_BUFFER_DESC vrtBufDsc;
    vrtBufDsc.ByteWidth = 2 * sizeof(float) * triangleVerts.size();
    vrtBufDsc.Usage = D3D11_USAGE_DEFAULT;
    vrtBufDsc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vrtBufDsc.CPUAccessFlags = 0;
    vrtBufDsc.MiscFlags = 0;
    vrtBufDsc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA vrtBufDat;
    vrtBufDat.pSysMem = triangleVerts.data();
    vrtBufDat.SysMemPitch = 0;
    vrtBufDat.SysMemSlicePitch = 0;

    throwIfFailed(device->CreateBuffer(&vrtBufDsc, &vrtBufDat, vrtBuffer.put()));


    std::vector<uint16_t> indicies{
        0, 1, 2
    };

    D3D11_BUFFER_DESC idxBufDsc;
    idxBufDsc.ByteWidth = sizeof(uint16_t) * indicies.size();
    idxBufDsc.Usage = D3D11_USAGE_DEFAULT;
    idxBufDsc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    idxBufDsc.CPUAccessFlags = 0;
    idxBufDsc.MiscFlags = 0;
    idxBufDsc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA idxBufDat;
    idxBufDat.pSysMem = indicies.data();
    idxBufDat.SysMemPitch = 0;
    idxBufDat.SysMemSlicePitch = 0;

    throwIfFailed(device->CreateBuffer(&idxBufDsc, &idxBufDat, idxBuffer.put()));
}

void Renderer::drawFrame() {
    ID3D11RenderTargetView* renderTargViews[] = { renderTarget.get() };
    context->OMSetRenderTargets(ARRAYSIZE(renderTargViews), renderTargViews, nullptr);

    float const clearColor[4] = { 0.f, 0.0f, 0.f, 1.f };
    context->ClearRenderTargetView(renderTarget.get(), clearColor);

    context->IASetInputLayout(inputLayout.get());


    uint32_t stride = sizeof(float);
    uint32_t offset = 0;

    ID3D11Buffer* vrtBuffers[] = { vrtBuffer.get() };

    context->IASetVertexBuffers(0, ARRAYSIZE(vrtBuffers), vrtBuffers, &stride, &offset);
    context->IASetIndexBuffer(idxBuffer.get(), DXGI_FORMAT_R16_UINT, 0);

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    context->VSSetShader(vtxShader.get(), nullptr, 0);
    context->PSSetShader(pxlShader.get(), nullptr, 0);

    context->DrawIndexed(3, 0, 0);

    
    
    throwIfFailed(swapChain->Present(1, 0));
}