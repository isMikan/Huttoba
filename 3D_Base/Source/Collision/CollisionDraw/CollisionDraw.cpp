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

        // weak_ptr をロックし、安全かつ短い名前の shared_ptr を取得
        auto pCollider = entry.pCollider.lock();

        // pCollider が有効でない場合は、次のエントリーへスキップ
        if (!pCollider)
        {
            continue;
        }

        // カプセルへのダウンキャストを試みる
        std::shared_ptr<const CollisionCapsule> pCapsule =
            std::dynamic_pointer_cast<const CollisionCapsule>(pCollider);

        // 共通: ワールド座標を設定
        const D3DXVECTOR3& drawPos = pCollider->GetWorldPosition();
        entry.pMesh->SetPosition(drawPos);

        // --- カプセル特有の処理 ---
        if (pCapsule)
        {
            entry.pMesh->SetPosition(pCapsule->GetDrawCenterPosition());
            entry.pMesh->SetQuaternion(pCapsule->GetDrawRotation());

            float length = pCapsule->GetDrawLength();
            float radius = pCapsule->GetWorldCapsule().Rad;
        }
        else // スフィアやその他の形状の場合
        {
            // カプセル以外は回転をリセット
            entry.pMesh->SetQuaternion(D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f));
        }

        //レンダリング
        entry.pMesh->Render(View, Proj, Light, Camera.vPosition);
    }
}