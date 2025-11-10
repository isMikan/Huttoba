#include "stdafx.h"
#include "CollisionDraw.h"


CollisionDraw::CollisionDraw()
	: m_pCollisionEntries{}
{
}

// 登録処理
void CollisionDraw::AddDrawMesh(
	const std::shared_ptr<CStaticMesh> pMesh,
	const CGameObject* pOwner,
    const CollisionBase* pCollider
)
{
	if (pMesh && pOwner)
	{
		m_pCollisionEntries.push_back({ pOwner, pMesh, pCollider });
	}
}

void CollisionDraw::RemoveByOwner(const CGameObject* pOwner)
{
    if (!pOwner) return;

    //受け取ったポインタと同じポインタを削除
    m_pCollisionEntries.erase(
        std::remove_if(m_pCollisionEntries.begin(), m_pCollisionEntries.end(),
            [pOwner](const DebugCollisionDrawEntry& entry) {
                return entry.pOwner == pOwner;
            }),
        m_pCollisionEntries.end());
}

void CollisionDraw::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
    for (auto& entry : m_pCollisionEntries)
    {
        if (!entry.pMesh || !entry.pOwner || !entry.pCollider) continue;

        const D3DXVECTOR3& drawPos = entry.pCollider->GetWorldPosition();

        entry.pMesh->SetPosition(drawPos);

        //カプセルだった場合
        const CollisionCapsule* pCapsule = dynamic_cast<const CollisionCapsule*>(entry.pCollider);
        if (pCapsule)
        {
            const D3DXVECTOR3& start = pCapsule->GetWorldCapsule().StartPoint;
            const D3DXVECTOR3& end = pCapsule->GetWorldCapsule().EndPoint;

            //単位ベクトルを取得
            D3DXVECTOR3 vAxisWorld = end - start;
            D3DXVec3Normalize(&vAxisWorld, &vAxisWorld);

            //回転を計算
            D3DXVECTOR3 vLocalAxis(0.0f, 1.0f, 0.0f);
            D3DXQUATERNION qRotation;

            D3DXVECTOR3 vAxis;
            float fDot = D3DXVec3Dot(&vLocalAxis, &vAxisWorld);

            if (fDot > 0.999f) // ほぼ同じ方向の場合（回転不要）
            {
                D3DXQuaternionIdentity(&qRotation);
            }
            else if (fDot < -0.999f) // ほぼ反対方向の場合（180度回転）
            {
                D3DXVECTOR3 vAxis180(0.0f, 0.0f, 1.0f);
                // 適切な垂直軸（例：Z軸）を中心に180度回転
                D3DXQuaternionRotationAxis(&qRotation, &vAxis180, D3DX_PI);
            }
            else // 一般的な回転
            {
                // 回転軸を外積で計算
                D3DXVec3Cross(&vAxis, &vLocalAxis, &vAxisWorld);
                D3DXVec3Normalize(&vAxis, &vAxis);

                // 回転角を内積の cos(theta) から計算
                float fAngle = acosf(fDot);

                // 軸と角度からクォータニオンを生成
                D3DXQuaternionRotationAxis(&qRotation, &vAxis, fAngle);
            }            // 4. 計算した回転をメッシュに適用
            entry.pMesh->SetQuaternion(qRotation);

            // スケールの適用 (カプセルメッシュの長さと半径を合わせる)
            float length = D3DXVec3Length(&vAxisWorld);
            float radius = pCapsule->GetWorldCapsule().Rad;
            // ... SetScaleで長さ(Y軸)と半径(X/Z軸)を調整するロジックが必要 ...
            // entry.pMesh->SetScale(radius, length, radius); 
        }
        else // スフィアやその他の形状の場合
        {
            // カプセル以外は回転をリセット（不要な回転が残らないように）
            entry.pMesh->SetQuaternion(D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f));
        }

        //レンダリング
        entry.pMesh->Render(View, Proj, Light, Camera.vPosition);
    }
}