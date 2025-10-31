#include "CollisionCapsule.h"

CollisionCapsule::CollisionCapsule(
	ICollisionListener* pListener, 
	const D3DXVECTOR3& syncPosition,
	ColliderTag tag,
	float radius, 
	const D3DXVECTOR3& localOffsetA, 
	const D3DXVECTOR3& localOffsetB, 
	const D3DXVECTOR3& localOffset
)
	: CollisionBase(pListener, syncPosition, tag, localOffset)
	, m_Radius(radius)
	, m_LocalOffsetA(localOffsetA)
	, m_LocalOffsetB(localOffsetB)
	, m_WorldCapsule({}) 
{
}


void CollisionCapsule::UpdateWorldPosition()
{
    // 1. 親オブジェクトのワールド行列を取得
    CGameObject* obj = dynamic_cast<CGameObject*>(GetListener());

    if (!obj)
    {
        // 親がいなければ判定不能
        SetActive(false);
        m_WorldPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

        std::cout << "親の取得に失敗 : 当たり判定(カプセル)" << std::endl;

        return;
    }

    // CGameObject::GetWorldMatrix() から安全で最新の行列を取得
    const D3DXMATRIX& mWorld = obj->GetWorldMatrix();

    // 軸線分の始点Aと終点Bをワールド行列で変換
    // D3DXVec3TransformCoord は D3DXMATRIXの回転、スケール、平行移動を全て適用
    D3DXVec3TransformCoord(&m_WorldCapsule.StartPoint, &m_LocalOffsetA, &mWorld);
    D3DXVec3TransformCoord(&m_WorldCapsule.EndPoint, &m_LocalOffsetB, &mWorld);

    // 半径の設定
    m_WorldCapsule.Rad = m_Radius;

    // CollisionBase の m_WorldPosition をカプセルの中心で更新
    m_WorldPosition = (m_WorldCapsule.StartPoint + m_WorldCapsule.EndPoint) / 2.0f;
}
