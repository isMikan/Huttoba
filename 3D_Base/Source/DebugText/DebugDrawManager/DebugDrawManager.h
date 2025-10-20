#pragma once
#include <d3d11.h>
#include <d3dx11.h> // D3DXMATRIX を使用
#include <vector>
#include <wrl/client.h> // ComPtr を使用

using Microsoft::WRL::ComPtr;

// D3DXと互換性のある頂点構造体
struct VertexPC
{
    float Position[3]; // D3DXVECTOR3 と同じメモリレイアウト (x, y, z)
    float Color[4];    // D3DXVECTOR4 と同じメモリレイアウト (r, g, b, a)
};

class DebugDrawManager
{
public:
    static DebugDrawManager* GetInstance()
    {
        static DebugDrawManager instance;
        return &instance;
    }

    // ComPtr でデバイスを受け取る (生のポインタでも可ですが、ここではComPtrを使用)
    void Init(ComPtr<ID3D11Device> device);
    void Release();

    // D3DXMATRIX を受け取る
    void Begin(ID3D11DeviceContext* context, const D3DXMATRIX& viewProj);

    // float* で座標と色を受け取る
    void DrawLine(const float* start, const float* end,
        const float* color);
    void End();

private:
    DebugDrawManager() = default;
    ~DebugDrawManager();

    // DirectXリソース (ComPtr化)
    ComPtr<ID3D11Device> m_Device;
    ComPtr<ID3D11DeviceContext> m_Context;

    ComPtr<ID3D11Buffer> m_VertexBuffer;
    ComPtr<ID3D11Buffer> m_ConstantBuffer;

    ComPtr<ID3D11VertexShader> m_VertexShader;
    ComPtr<ID3D11PixelShader> m_PixelShader;
    ComPtr<ID3D11InputLayout> m_InputLayout;

    std::vector<VertexPC> m_Vertices;
    D3DXMATRIX m_ViewProj; // D3DXMATRIX を使用
};