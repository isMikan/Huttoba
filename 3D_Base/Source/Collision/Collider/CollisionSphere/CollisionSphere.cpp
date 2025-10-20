#include "stdafx.h"
#include "CollisionSphere.h"
#include <d3dx9math.h>

CollisionSphere::CollisionSphere()
    : m_LocalCenter(0.0f, 0.0f, 0.0f)
    , m_WorldCenter(0.0f, 0.0f, 0.0f)
    , m_Radius(1.0f)
    , m_WorldRadius(1.0f)
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

    //ワールド半径を更新 (ローカル半径 * 最大ワールドスケール)
    float maxScale = std::max(m_vScale.x, std::max(m_vScale.y, m_vScale.z));
    m_WorldRadius = m_Radius * maxScale;
}

void CollisionSphere::DrawDebug()
{
    DebugDrawManager* ddm = DebugDrawManager::GetInstance();

    const int segments = 24;
    const float step = D3DX_PI * 2.0f / segments;

    // 色設定（衝突中は赤、それ以外は白）
    float colorNormal[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float colorHit[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
    const float* color = (m_IsHit) ? colorHit : colorNormal;

    // ワールド座標上の中心と半径
    // m_WorldCenter のみを使用し、回転は無視する
    D3DXVECTOR3 center = m_WorldCenter;
    float r = m_WorldRadius; 

    // 3軸（XY / XZ / YZ）平面の輪を描く
    for (int plane = 0; plane < 3; ++plane)
    {
        D3DXVECTOR3 prev, curr;

        // 初期点設定 (t=0)
        float t = 0.0f;

        if (plane == 0) { // XY平面 (Zは固定)
            prev = D3DXVECTOR3(center.x + r * cosf(t), center.y + r * sinf(t), center.z);
        }
        else if (plane == 1) { // XZ平面 (Yは固定)
            prev = D3DXVECTOR3(center.x + r * cosf(t), center.y, center.z + r * sinf(t));
        }
        else { // YZ平面 (Xは固定)
            prev = D3DXVECTOR3(center.x, center.y + r * cosf(t), center.z + r * sinf(t));
        }

        // 線分描画ループ
        for (int i = 1; i <= segments; ++i)
        {
            float t2 = i * step;

            if (plane == 0)
                curr = D3DXVECTOR3(center.x + r * cosf(t2), center.y + r * sinf(t2), center.z);
            else if (plane == 1)
                curr = D3DXVECTOR3(center.x + r * cosf(t2), center.y, center.z + r * sinf(t2));
            else
                curr = D3DXVECTOR3(center.x, center.y + r * cosf(t2), center.z + r * sinf(t2));

            // float* に変換して描画
            DebugDrawManager::GetInstance()->DrawLine((float*)&prev, (float*)&curr, color);

            prev = curr;
        }

        // ★注: 最後の線分 (curr(t2=2π) から最初の点 (t=0) まで) が描画されていない
        // 最後に prev (最後の点) から初期点 (t=0) へ線分を追加する必要がある場合があります。
        // ただし、i <= segments のループ構成の場合、i=segments のループで最初の点に非常に近い点が計算されているため、
        // ほとんどの場合、見た目の問題はありません。
    }
}