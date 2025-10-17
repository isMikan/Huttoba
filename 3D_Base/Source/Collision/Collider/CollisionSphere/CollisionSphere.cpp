#include "stdafx.h"
#include "CollisionSphere.h"
#include <d3dx9math.h>

CollisionSphere::CollisionSphere()
    : m_LocalCenter(0.0f, 0.0f, 0.0f)
    , m_WorldCenter(0.0f, 0.0f, 0.0f)
    , m_Radius(1.0f)
{
    D3DXMatrixIdentity(&m_World);
}

CollisionSphere::~CollisionSphere()
{
}

void CollisionSphere::UpdateWorldMat()
{
    // スケール・回転・平行移動を反映
    D3DXMATRIX matS, matR, matT;
    D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
    D3DXMatrixRotationYawPitchRoll(&matR, m_vRotation.y, m_vRotation.x, m_vRotation.z);
    D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

    m_World = matS * matR * matT;

    // ワールド座標上の中心更新
    D3DXVec3TransformCoord(&m_WorldCenter, &m_LocalCenter, &m_World);
}
