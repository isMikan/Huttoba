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
	//--- クラスがプレイヤーの場合の関数 ---.
	void ChildPlayer(int index);

	//--- ダウン時間を減少する関数 ---.
	void TimeDecrease();

	//--- 入力を受け付けるか判断する関数 ---.
	bool IsInput(float x, float y, int index) const;

private:
	float	m_StartTime;			//開始時間.
	float	m_EndTime;				//終了時間.
	float	m_MaxTime;				//最大時間.

	float	m_DecreaseTriggerTime;	//減少が開始時間.
	float	m_TimeDecrease;			//減少する時間.
	float	m_TimeDecreaseByMashing;//連打操作によって減少する時間.

	float	m_ShakeSpeed;			//揺れる速度.
	float	m_ShakeWidth;			//揺れる幅.

	float	m_PrevSthikX;			//以前に入力されたスティックX軸.
	float	m_PrevSthikY;			//以前に入力されたスティックY軸.

	bool	m_IsTimeDecreasing;		//時間を減少するか.

	D3DXQUATERNION	m_StartQuat;	//開始時の回転.

	D3DXVECTOR3		m_RightHandPos;	//右手の元の位置.
	D3DXVECTOR3		m_LeftHandPos;	//左手の元の位置.

private:
	CPlayerKnockdownState() = delete;	//デフォルトコンストラクタ禁止.
	CPlayerKnockdownState(const CPlayerKnockdownState&) = delete;
	CPlayerKnockdownState& operator = (const CPlayerKnockdownState& rhs) = delete;

};