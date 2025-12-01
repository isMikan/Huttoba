#pragma once

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

	//追尾から無視するプレイヤーを設定
	void SetIgnoredPlayer(CPlayerBase* pIgnoredPlayer) { m_pIgnoredPlayer = pIgnoredPlayer; }

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

	//当たり判定のコライダーの数を指定
	static constexpr int m_ColliderCount = 3;

	//当たり判定を複数
	std::array<std::shared_ptr<CollisionBase>, m_ColliderCount>	m_pCollision;

	//索敵の当たり判定の位置の補正値
	D3DXVECTOR3 m_CollisionOffSet;
};