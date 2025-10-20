#include "DebugDrawManager.h"
#include <d3dcompiler.h>
#include <cassert>
#include <cstring> // memcpy のために必要

// --------------------------------------------------------------------------------
// デストラクタ
// --------------------------------------------------------------------------------
DebugDrawManager::~DebugDrawManager()
{
    Release();
}

// --------------------------------------------------------------------------------
// 内部で使用するシェーダーコード (文字列で定義)
// --------------------------------------------------------------------------------
namespace
{
    // 定数バッファ構造体 (D3DXMATRIXのサイズと同じ)
    struct CbPerFrame
    {
        D3DXMATRIX ViewProj;
    };

    // 頂点シェーダー (VS)
    const char* g_VSSource = R"(
        cbuffer CbPerFrame : register(b0)
        {
            matrix ViewProj; // HLSL の matrix は 4x4
        };

        struct VSInput
        {
            float3 Position : POSITION;
            float4 Color    : COLOR;
        };

        struct PSInput
        {
            float4 Position : SV_POSITION;
            float4 Color    : COLOR;
        };

        PSInput main(VSInput input)
        {
            PSInput output;
            // WVP行列で変換
            output.Position = mul(float4(input.Position, 1.0f), ViewProj); 
            output.Color = input.Color;
            return output;
        }
    )";

    // ピクセルシェーダー (PS)
    const char* g_PSSource = R"(
        struct PSInput
        {
            float4 Position : SV_POSITION;
            float4 Color    : COLOR;
        };

        float4 main(PSInput input) : SV_TARGET
        {
            return input.Color;
        }
    )";
}

// --------------------------------------------------------------------------------
// 実装
// --------------------------------------------------------------------------------

void DebugDrawManager::Init(ComPtr<ID3D11Device> device)
{
    m_Device = device;

    // 1. 頂点バッファ作成 (最大1000頂点 = 500本の線分)
    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.Usage = D3D11_USAGE_DYNAMIC;
    vbDesc.ByteWidth = sizeof(VertexPC) * 1000;
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    HRESULT hr = m_Device->CreateBuffer(&vbDesc, nullptr, m_VertexBuffer.GetAddressOf());
    assert(SUCCEEDED(hr));

    // 2. 定数バッファ作成 (CbPerFrame用)
    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.ByteWidth = sizeof(CbPerFrame);
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    hr = m_Device->CreateBuffer(&cbDesc, nullptr, m_ConstantBuffer.GetAddressOf());
    assert(SUCCEEDED(hr));

    // 3. シェーダーのコンパイルと作成
    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* psBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;

    // 頂点シェーダーコンパイル
    hr = D3DCompile(g_VSSource, strlen(g_VSSource), nullptr, nullptr, nullptr, "main", "vs_5_0", 0, 0, &vsBlob, &errorBlob);
    if (FAILED(hr)) {
        OutputDebugStringA((char*)errorBlob->GetBufferPointer());
        assert(false);
    }
    hr = m_Device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, m_VertexShader.GetAddressOf());
    assert(SUCCEEDED(hr));

    // ピクセルシェーダーコンパイル
    hr = D3DCompile(g_PSSource, strlen(g_PSSource), nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, &psBlob, &errorBlob);
    if (FAILED(hr)) {
        OutputDebugStringA((char*)errorBlob->GetBufferPointer());
        assert(false);
    }
    hr = m_Device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, m_PixelShader.GetAddressOf());
    assert(SUCCEEDED(hr));

    // 4. インプットレイアウトの作成
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                      D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    hr = m_Device->CreateInputLayout(layout, ARRAYSIZE(layout),
        vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), m_InputLayout.GetAddressOf());
    assert(SUCCEEDED(hr));

    // Blob解放
    if (vsBlob) vsBlob->Release();
    if (psBlob) psBlob->Release();
    if (errorBlob) errorBlob->Release();
}

void DebugDrawManager::Release()
{
    // 一応明示的にリセット
    m_Device.Reset();
    m_Context.Reset();
    m_VertexBuffer.Reset();
    m_ConstantBuffer.Reset();
    m_VertexShader.Reset();
    m_PixelShader.Reset();
    m_InputLayout.Reset();
}

void DebugDrawManager::Begin(ID3D11DeviceContext* context, const D3DXMATRIX& viewProj)
{
    // ID3D11DeviceContext* は生ポインタとして受け取り、ComPtrに代入
    m_Context = context;

    m_ViewProj = viewProj; // D3DXMATRIX の代入
    m_Vertices.clear();
}

void DebugDrawManager::DrawLine(const float* start,
    const float* end,
    const float* color)
{
    VertexPC vStart, vEnd;

    // start, end, color (float[3] or float[4]) を VertexPC にコピー
    memcpy(vStart.Position, start, sizeof(float) * 3);
    memcpy(vStart.Color, color, sizeof(float) * 4);
    memcpy(vEnd.Position, end, sizeof(float) * 3);
    memcpy(vEnd.Color, color, sizeof(float) * 4);

    m_Vertices.push_back(vStart);
    m_Vertices.push_back(vEnd);
}

void DebugDrawManager::End()
{
    if (m_Vertices.empty()) return;

    // CDirectX11のインスタンスを取得
    CDirectX11* dx11 = CDirectX11::GetInstance();

    // 深度テストを無効 (常に手前に描画するため)
    dx11->SetDepth(false);
    // アルファブレンドを有効 (デバッグ線の色が透過する可能性に備えるため)
    dx11->SetAlphaBlend(true);


    D3D11_BUFFER_DESC desc;
    m_VertexBuffer->GetDesc(&desc);

    // 確保サイズチェック
    if (m_Vertices.size() * sizeof(VertexPC) > desc.ByteWidth) {
        OutputDebugStringA("DebugDrawManager: Vertex buffer capacity exceeded!\n");

        dx11->SetDepth(false);
        dx11->SetAlphaBlend(true);
        return;
    }

    // 頂点データをGPUに転送 (m_VertexBuffer.Get())
    D3D11_MAPPED_SUBRESOURCE mappedVB = {};
    HRESULT hr = m_Context->Map(m_VertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVB);
    assert(SUCCEEDED(hr));
    memcpy(mappedVB.pData, m_Vertices.data(), sizeof(VertexPC) * m_Vertices.size());
    m_Context->Unmap(m_VertexBuffer.Get(), 0);

    //  定数バッファの更新と設定 (m_ConstantBuffer.Get())
    D3D11_MAPPED_SUBRESOURCE mappedCB = {};
    hr = m_Context->Map(m_ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedCB);
    assert(SUCCEEDED(hr));

    CbPerFrame cbufferData;
    // D3DXMatrixTranspose を使用して転置し、定数バッファに格納
    D3DXMatrixTranspose(&cbufferData.ViewProj, &m_ViewProj);

    memcpy(mappedCB.pData, &cbufferData, sizeof(CbPerFrame));
    m_Context->Unmap(m_ConstantBuffer.Get(), 0);

    // パイプラインステートの設定
    UINT stride = sizeof(VertexPC);
    UINT offset = 0;

    // Get() で生のポインタを取得して設定
    ID3D11Buffer* pVB = m_VertexBuffer.Get();
    m_Context->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);
    m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    m_Context->IASetInputLayout(m_InputLayout.Get());
    m_Context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
    m_Context->PSSetShader(m_PixelShader.Get(), nullptr, 0);

    ID3D11Buffer* pCB = m_ConstantBuffer.Get();
    m_Context->VSSetConstantBuffers(0, 1, &pCB);

    // 描画実行
    m_Context->Draw(static_cast<UINT>(m_Vertices.size()), 0);

    // シーンのデフォルト設定（深度ON、ブレンドOFFなど）に戻す
    dx11->SetDepth(true);
    dx11->SetAlphaBlend(false);
}