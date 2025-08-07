#pragma once

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/PlayerState/ActionState/CActionState.h"

class CPlayerHandAttack
	: public CActionState
{
public:
	CPlayerHandAttack();
	~CPlayerHandAttack();

	//状態が始めるときだけ呼び出される.
	void Enter(CPlayer& pPlayer) override;
	//状態が終わるときだけ呼び出される.
	void Exit(CPlayer& pPlayer) override;
	//この状態の間は毎フレーム呼び出される.
	void Update(CPlayer& pPlayer) override;

private:
	D3DXVECTOR3 m_RightOffset;	//右手の調整位置.
	D3DXVECTOR3 m_LeftOffset;	//左手の調整位置.
	D3DXVECTOR3 m_RightPos;		//右手の位置.
	D3DXVECTOR3 m_LeftPos;		//左手の位置.

	float	m_CenterOffset;		//中心に寄りに調整.

	float	m_StartTime;		//開始時間.
	float	m_EndTime;			//終了時間.

	float	m_CurrentSpeed;		//現在の速度.
	float	m_AttackSpeed;		//攻撃速度.
	float	m_MovingAtkSpeed;	//移動している時の攻撃速度.
};