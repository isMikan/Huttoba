#pragma once

#include "CStaticMeshObject.h"
#include "Ground/GroundManager/CGroundManager.h"

//地面の当たり判定の生成と更新クラス
class CGroundCollisionProxy
	: public CStaticMeshObject
	, public ICollisionListener
{
public:
	CGroundCollisionProxy(CGroundManager& groundManager);
	~CGroundCollisionProxy();

	void Update();

private:
	//当たり判定処理
	virtual void OnCollision(CollisionBase* other)override;

private:
	CGroundManager&	m_pGroundManager;

	//出現中の地面の数
	size_t	m_GroundCount;

	//1フレーム前に出現していた地面の数
	size_t	m_OldGroundCount;
};