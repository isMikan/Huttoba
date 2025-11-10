#include "stdafx.h"
#include "CollisionDraw.h"
#include "Collision/Collider/CollisionCapsule/CollisionCapsule.h"

CollisionDraw::CollisionDraw()
	: m_pCollisionEntries{}
{
}

// 登録処理
void CollisionDraw::AddDrawMesh(
	const std::shared_ptr<CStaticMesh> pMesh,
	const CGameObject* pOwner,
    std::weak_ptr<const CollisionBase> pCollider
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
        if (!entry.pMesh || !entry.pOwner) continue;

        // 1. 【修正1】 weak_ptr をロックし、安全かつ短い名前の shared_ptr を取得
        // entry.pCollider は entry.pColliderWeak に修正済みと仮定
        auto pCollider = entry.pCollider.lock();

        // 2. 【修正1】 pCollider が有効でない場合は、次のエントリーへスキップ
        if (!pCollider)
        {
            continue;
        }

        // 3. 【修正2】 カプセルへのダウンキャストを試みる
        std::shared_ptr<const CollisionCapsule> pCapsule =
            std::dynamic_pointer_cast<const CollisionCapsule>(pCollider);

        // 共通: ワールド座標を設定
        const D3DXVECTOR3& drawPos = pCollider->GetWorldPosition();
        entry.pMesh->SetPosition(drawPos);

        // --- カプセル特有の処理 ---
        if (pCapsule)
        {
            // const参照で安全に取得（以前の議論で修正済み）
            const D3DXVECTOR3& start = pCapsule->GetWorldCapsule().StartPoint;
            const D3DXVECTOR3& end = pCapsule->GetWorldCapsule().EndPoint;

            // ... (回転計算ロジックは正しいので省略) ...

            D3DXVECTOR3 vAxisWorld = end - start;
            D3DXVec3Normalize(&vAxisWorld, &vAxisWorld);
            D3DXVECTOR3 vLocalAxis(0.0f, 1.0f, 0.0f);
            D3DXQUATERNION qRotation;

            D3DXVECTOR3 vAxis;
            float fDot = D3DXVec3Dot(&vLocalAxis, &vAxisWorld);

            if (fDot > 0.999f) D3DXQuaternionIdentity(&qRotation);
            else if (fDot < -0.999f)
            {
                D3DXVECTOR3 vAxis180(0.0f, 0.0f, 1.0f);
                D3DXQuaternionRotationAxis(&qRotation, &vAxis180, D3DX_PI);
            }
            else
            {
                D3DXVec3Cross(&vAxis, &vLocalAxis, &vAxisWorld);
                D3DXVec3Normalize(&vAxis, &vAxis);
                float fAngle = acosf(fDot);
                D3DXQuaternionRotationAxis(&qRotation, &vAxis, fAngle);
            }

            // 4. 回転をメッシュに適用
            entry.pMesh->SetQuaternion(qRotation);

            // 5. スケール適用 (省略されているロジックは別途実装してください)
            // float length = D3DXVec3Length(&(end - start)); // 正しい長さの計算
            // float radius = pCapsule->GetWorldCapsule().Rad;
            // entry.pMesh->SetScale(radius, length, radius); 
        }
        else // スフィアやその他の形状の場合
        {
            // 5. カプセル以外は回転をリセット
            entry.pMesh->SetQuaternion(D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f));
            // スケールもここでリセット/設定 (entry.pMesh->SetScale(radius, radius, radius); など)
        }

        //レンダリング
        entry.pMesh->Render(View, Proj, Light, Camera.vPosition);
    }
}