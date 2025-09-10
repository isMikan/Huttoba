#include "stdafx.h"
#include "CollisionManager.h"
#include "Collision/CBoundingSphere.h"
#include "CCharacter/CPlayer/CPlayer.h"
#include "Item/ItemBase.h"

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

bool CollisionManager::IsHit(const CBoundingSphere& pBSphere)
{
	////２つの球体の中心間の距離を求める
	//D3DXVECTOR3 vLength = m_Position - pBSphere.GetPosition();
	////上記のベクトルから長さに変換
	//float Length = D3DXVec3Length(&vLength);

	////「２つの球体の距離」が「２つの球体のそれぞれの半径を足したもの」より、
	////小さいということは、球体同士が重なっている（衝突している）ということ
	//if (Length <= m_Radius + pBSphere.GetRadius())
	//{
	//	return true;	//衝突している
	//}
	//return false;	//衝突していない

	return false;
}
