#pragma once
//#include "Collision/"

class CPlayerBase;

class ChaseSensor
	: public CStaticMeshObject, public ICollisionListener
{
public:
	ChaseSensor(D3DXVECTOR3 pos,D3DXVECTOR3 searchOffset);
	~ChaseSensor();

	//一番近いプレイヤーを渡す
	CPlayerBase* GetTarget() { return m_pTarget; }

	bool GetIsHitGround() { return m_IsHitGround; }

	//索敵のセンサの設定
	void SetIsSensorActive(bool sensorActive) { m_IsSensorActive = sensorActive; }

	//一番近いプレイヤーを判定
	void FindNearestTarget();

private:
	//当たり判定処理
	virtual void OnCollision(CollisionBase* other)override;


private:
	//索敵判定にあたったプレイヤーをすべて記録
	std::vector<CPlayerBase*> m_pTargetList;
	
	//当たった一番近いプレイヤーを記録
	CPlayerBase* m_pTarget;

	std::shared_ptr<CollisionBase>	m_pCollision;

	//索敵の当たり判定の位置の補正値
	D3DXVECTOR3 m_CollisionOffSet;

	//索敵のセンサーのON/Off切り替え
	bool m_IsSensorActive;

	//地面の当たり判定と当たっているか
	bool m_IsHitGround;
};