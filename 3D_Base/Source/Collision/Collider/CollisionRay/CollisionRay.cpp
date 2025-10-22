//#include "stdafx.h"
//#include "CollisionRay.h"
//
//CollisionRay::CollisionRay()
//    : m_WorldOrigin(0.0f, 0.0f, 0.0f)
//    , m_LocalOrigin(0.0f, 0.0f, 0.0f)
//    , m_LocalDirection(0.0f, -1.0f, 0.0f) // ローカル方向をデフォルトに設定
//    , m_Direction(0.0f, 0.0f, 0.0f)       // ワールド方向は初期化時に空、UpdateWorldMatで設定
//    , m_Length(10.0f)
//{
//    D3DXMatrixIdentity(&m_World);
//}
//
//CollisionRay::~CollisionRay()
//{
//}
//
//void CollisionRay::UpdateWorldMat()
//{
//    // スケール・回転・平行移動を反映（他のColliderと統一）
//    D3DXMATRIX matS, matR, matT;
//    D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
//    D3DXMatrixRotationYawPitchRoll(&matR, m_vRotation.y, m_vRotation.x, m_vRotation.z);
//    D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
//
//    m_World = matS * matR * matT;
//
//    // 1. 始点のワールド座標を更新
//    D3DXVec3TransformCoord(&m_WorldOrigin, &m_LocalOrigin, &m_World);
//
//    // 2. 方向ベクトルのワールド座標への変換 (回転とスケールのみ適用)
//    D3DXVECTOR3 tempDirection;
//    D3DXVec3TransformNormal(&tempDirection, &m_LocalDirection, &m_World);
//
//    // 3. ワールド変換後の方向を正規化して m_Direction に格納
//    // レイの方向は必ず単位ベクトルである必要がある
//    D3DXVec3Normalize(&m_Direction, &tempDirection);
//}
//
//D3DXVECTOR3 CollisionRay::GetEndPoint() const
//{
//    // 始点 + 方向 × 長さ
//    return m_WorldOrigin + m_Direction * m_Length;
//}
