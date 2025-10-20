//// DebugDrawManager.h
//#pragma once
//#include <memory>
//#include <DirectXMath.h>
//#include <DirectXColors.h>
//#include <SimpleMath.h>
//#include <PrimitiveBatch.h>
//#include <VertexTypes.h>
//#include <Effects.h>
//
//class DebugDrawManager
//{
//public:
//    static DebugDrawManager* GetInstance();
//
//    void Initialize(ID3D11Device* device);
//    void Begin(ID3D11DeviceContext* context, const DirectX::XMMATRIX& viewProj);
//    void DrawLine(const DirectX::XMFLOAT3& start,
//        const DirectX::XMFLOAT3& end,
//        const DirectX::XMFLOAT4& color = DirectX::Colors::Lime);
//    void End();
//
//private:
//    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_Batch;
//    std::unique_ptr<DirectX::BasicEffect> m_Effect;
//    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
//    ID3D11DeviceContext* m_Context = nullptr;
//};
