#pragma once
//#include "Collision/"

class CPlayerBase;

class ChaseSensor
	: public CStaticMeshObject, public ICollisionListener
{
public:
	ChaseSensor(D3DXVECTOR3 pos,D3DXVECTOR3 searchOffset);
	~ChaseSensor();

	void Update()override;

	//一番近いプレイヤーを渡す
	CPlayerBase* GetTarget() { return m_pTarget; }

	void SetIgnoredPlayer(CPlayerBase* pIgnoredPlayer) { m_pIgnoredPlayer = pIgnoredPlayer; }

	bool GetIsHitGround() { return m_IsHitGround; }
	void SetIsHitGround(bool IsHitGround) { m_IsHitGround = IsHitGround; }

	void SetIsCollisionEnabled(bool set) { m_IsCollisionEnabled = set; }

private:
	//当たり判定処理
	virtual void OnCollision(CollisionBase* other)override;

	//一番近いプレイヤーを判定
	void FindNearestTarget();

private:
	//索敵判定にあたったプレイヤーをすべて記録
	std::vector<CPlayerBase*> m_pTargetList;
	
	//当たった一番近いプレイヤーを記録
	CPlayerBase* m_pTarget;

	//追尾から無視するプレイヤー
	CPlayerBase* m_pIgnoredPlayer;

	std::shared_ptr<CollisionBase>	m_pCollision;

	//索敵の当たり判定の位置の補正値
	D3DXVECTOR3 m_CollisionOffSet;

	//地面の当たり判定と当たっているか
	bool m_IsHitGround;

	//当たり判定の処理が有効か
	bool	m_IsCollisionEnabled;
};