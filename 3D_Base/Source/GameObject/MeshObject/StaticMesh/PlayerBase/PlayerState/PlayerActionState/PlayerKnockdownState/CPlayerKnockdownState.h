#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/CPlayerState.h"

/**************************************************
*	プレイヤーの起き上がる状態クラス.
**/
class CPlayerKnockdownState
	: public CPlayerState	//プレイヤー状態を継承.
{
public:
	CPlayerKnockdownState(CPlayerBase& pPlayer);
	~CPlayerKnockdownState();

	//--- 状態の開始時に呼び出す関数 ---.
	void Enter() override;
	//--- 状態の終了時に呼び出す関数 ---.
	void Exit() override;
	//--- この状態の間に呼び出す関数 ---.
	void Update() override;

private:

	void ChildPlayer(int index);


	void DecreaseTime();

private:
	float	m_StartTime;			//開始時間.
	float	m_EndTime;				//終了時間.

	float	m_DecreaseTriggerTime;	//減少が開始時間.
	float	m_DecreaseTime;			//減少する時間.

	float	m_ShakeSpeed;			//揺れる速度.
	float	m_ShakeWidth;			//揺れる幅.

	bool	m_IsTimeDecreasing;		//時間を減少するか.

	D3DXQUATERNION	m_StartQuat;	//開始時の回転.

	D3DXVECTOR3		m_RightHandPos;	//右手の元の位置.
	D3DXVECTOR3		m_LeftHandPos;	//左手の元の位置.

private:
	CPlayerKnockdownState() = delete;	//デフォルトコンストラクタ禁止.
	CPlayerKnockdownState(const CPlayerKnockdownState&) = delete;
	CPlayerKnockdownState& operator = (const CPlayerKnockdownState& rhs) = delete;

};