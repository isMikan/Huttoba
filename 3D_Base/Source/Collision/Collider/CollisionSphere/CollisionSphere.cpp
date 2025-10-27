#include "CollisionSphere.h"

CollisionSphere::CollisionSphere(
    ICollisionListener* pListener,
    const D3DXVECTOR3&  syncPosition,
    ColliderTag         tag,
    float               radius,
    const D3DXVECTOR3&  localOffset
) 
    : CollisionBase(pListener, syncPosition, tag, localOffset),
      m_Radius(radius)
{
}

void CollisionSphere::AttachDebugMesh(std::shared_ptr<CStaticMesh> pMesh)
{
    m_pDebugMesh = pMesh;

    // デフォルトのデバッグ描画色を設定
    m_DebugColor.diffuse = D3DXVECTOR4(0.0f, 0.8f, 0.0f, 0.3f); // 透明な緑
    m_DebugColor.ambient = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.0f);
    m_DebugColor.specular = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
}

// デバッグ描画を実行
void CollisionSphere::DrawDebug(
    D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) const
{
    // メッシュが設定されていない、または非アクティブなら描画しない
    if (!m_pDebugMesh || !GetActive()) return;

    // スケーリング行列の計算 (半径に合わせて拡大)
    D3DXMATRIX matScale;
    D3DXMatrixScaling(&matScale, m_Radius, m_Radius, m_Radius);

    // 平行移動行列の計算 (コライダーの中心位置に移動)
    D3DXMATRIX matTrans;
    D3DXMatrixTranslation(&matTrans,
        m_WorldPosition.x, m_WorldPosition.y, m_WorldPosition.z);

    // ワールド行列の合成
    D3DXMATRIX matWorld = matScale * matTrans;

}