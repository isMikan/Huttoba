#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/CPlayerState.h"

/**************************************************
*	プレイヤーが押される状態クラス.
**/
class CPlayerPushedState
	: public CPlayerState	//プレイヤー状態継承クラス.
{
public:
	CPlayerPushedState(CPlayerBase& pPlayer);
	~CPlayerPushedState() override;

	//--- 通知 ---.
	void OnNotify(IPlayerObserver::PlayerEvent event) override;

	//--- 状態の開始時に呼び出す関数 ---.
	void Enter() override;
	//--- 状態の終了時に呼び出す関数 ---.
	void Exit() override;
	//--- この状態の間に呼び出す関数 ---.
	void Update() override;

private:
	float			m_StartTime;			//開始時間.
	float			m_EndTime;				//終了時間.

	float			m_CurrentTiltAngle;		//現在の傾き角度.
	float			m_TiltAngleMax;			//最大の傾き角度.
	float			m_PhaseSplit;			//傾きの変わり目.

	D3DXVECTOR3		m_RightHandStartPos;	//右手の元の位置.
	D3DXVECTOR3		m_LeftHandStartPos;		//左手の元の位置.
	D3DXVECTOR3		m_RightHandEndPos;		//右手の元の位置.
	D3DXVECTOR3		m_LeftHandEndPos;		//左手の元の位置.

	D3DXQUATERNION	m_StartQuat;			//開始時の回転.

private:
	CPlayerPushedState() = delete;	//デフォルトコンストラクタ禁止.
	CPlayerPushedState(const CPlayerPushedState&) = delete;
	CPlayerPushedState& operator = (const CPlayerPushedState& rhs) = delete;

};