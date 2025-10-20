#include "TestScene.h"
#include <d3dx9.h>

CollisionTestScene::CollisionTestScene()
{
    Create();
}

CollisionTestScene::~CollisionTestScene()
{
}

HRESULT CollisionTestScene::Create()
{
    // ViewProj行列（固定視点カメラ）
    D3DXVECTOR3 eye(0.0f, 5.0f, -10.0f);
    D3DXVECTOR3 at(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

    D3DXMATRIX view, proj;
    D3DXMatrixLookAtLH(&view, &eye, &at, &up);
    D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 4.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    m_ViewProj = view * proj;

    // Sphere A
    m_SphereA = std::make_unique<CollisionSphere>();
    m_SphereA->SetCenter(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
    m_SphereA->SetRadius(1.0f);

    // Sphere B
    m_SphereB = std::make_unique<CollisionSphere>();
    m_SphereB->SetCenter(D3DXVECTOR3(3.0f, 0.0f, 0.0f)); // 初期は非衝突
    m_SphereB->SetRadius(1.0f);

    // CollisionManagerへ登録
    CollisionManager::GetInstance()->AddCollider(m_SphereA.get());
    CollisionManager::GetInstance()->AddCollider(m_SphereB.get());

    return S_OK;
}

HRESULT CollisionTestScene::LoadData()
{
    return S_OK;
}

void CollisionTestScene::Destroy()
{

}


void CollisionTestScene::Update()
{
    // Bを少しずつ左へ移動 → 衝突させる
    D3DXVECTOR3 center = m_SphereB->GetCenter();
    center.x -= 0.02f;
    m_SphereB->SetPosition(D3DXVECTOR3(center.x,0,0));

    CollisionManager::GetInstance()->CheckCollisions();
}

void CollisionTestScene::Draw()
{
    auto pContext = CDirectX11::GetInstance()->GetContext(); // ※DX11環境のままなら仮
    // DX9なら LPDIRECT3DDEVICE9 デバイスを渡すようにする

    DebugDrawManager::GetInstance()->Begin(pContext, m_ViewProj);
    m_SphereA->DrawDebug();
    m_SphereB->DrawDebug();
    DebugDrawManager::GetInstance()->End();
}
