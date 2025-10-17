#include "stdafx.h"
#include "CollisionRay.h"

CollisionRay::CollisionRay()
    : m_WorldOrigin(0.0f, 0.0f, 0.0f)
    , m_LocalOrigin(0.0f, 0.0f, 0.0f)
    , m_Direction(0.0f, -1.0f, 0.0f) // 下方向をデフォルト
    , m_Length(10.0f)
{
    D3DXMatrixIdentity(&m_World);
    NormalizeDirection();
}

CollisionRay::~CollisionRay()
{
}

void CollisionRay::UpdateWorldMat()
{
    // スケール・回転・平行移動を反映（他のColliderと統一）
    D3DXMATRIX matS, matR, matT;
    D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
    D3DXMatrixRotationYawPitchRoll(&matR, m_vRotation.y, m_vRotation.x, m_vRotation.z);
    D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

    m_World = matS * matR * matT;

    // 始点のワールド座標を更新
    D3DXVec3TransformCoord(&m_WorldOrigin, &m_LocalOrigin, &m_World);
}

void CollisionRay::NormalizeDirection()
{
    if (D3DXVec3LengthSq(&m_Direction) > 0.0001f)
        D3DXVec3Normalize(&m_Direction, &m_Direction);
}

D3DXVECTOR3 CollisionRay::GetEndPoint() const
{
    // 始点 + 方向 × 長さ
    return m_WorldOrigin + m_Direction * m_Length;
}
