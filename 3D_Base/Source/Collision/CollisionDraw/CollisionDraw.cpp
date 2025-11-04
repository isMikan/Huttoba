#include "stdafx.h"
#include "CollisionDraw.h"


CollisionDraw::CollisionDraw()
	: m_pCollisionEntries{}
{
}

// 登録処理
void CollisionDraw::AddDrawMesh(
	const std::shared_ptr<CStaticMesh> pMesh,
	const CGameObject* pOwner)
{
	if (pMesh && pOwner)
	{
		m_pCollisionEntries.push_back({ pOwner, pMesh });
	}
}

void CollisionDraw::RemoveByOwner(const CGameObject* pOwner)
{

    if (!pOwner) return;

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

        // 位置の追従
        entry.pMesh->SetPosition(entry.pOwner->GetPosition());

        // 回転の追従 (クォータニオンとオイラー角のどちらか、または両方をコピー)
        // クォータニオンが最終的な回転に使われているなら、こちらを優先
        entry.pMesh->SetQuaternion(entry.pOwner->GetQuaternion());

        // 拡縮の追従
        entry.pMesh->SetScale(entry.pOwner->GetScale());

        //レンダリング。
        entry.pMesh->Render(View, Proj, Light, Camera.vPosition);
    }
}