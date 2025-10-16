#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/CPlayerState.h"

/**************************************************
*	プレイヤーがアイテム未所持の攻撃状態クラス.
**/
class CPlayerHandAttackState
	: public CPlayerState	//プレイヤー状態継承クラス.
{
public:
	CPlayerHandAttackState(CPlayerBase& pPlayer);
	~CPlayerHandAttackState();

	//--- 通知 ---.
	void OnNotify(IPlayerObserver::PlayerEvent event) override;

	//--- 状態の開始時に呼び出す関数 ---.
	void Enter() override;
	//--- 状態の終了時に呼び出す関数 ---.
	void Exit() override;
	//--- この状態の間に呼び出す関数 ---.
	void Update() override;

private:
	float			m_CenterHandOffset;		//手を中心寄りに調整.

	float			m_StartTime;			//開始時間.
	float			m_EndTime;				//終了時間.

	float			m_CurrentTiltAngle;		//現在の傾き角度.
	float			m_TiltAngleMax;			//最大の傾き角度.

	D3DXVECTOR3		m_RightHandStartPos;	//右手の元の位置.
	D3DXVECTOR3		m_LeftHandStartPos;		//左手の元の位置.
	D3DXVECTOR3		m_RightHandEndPos;		//右手の元の位置.
	D3DXVECTOR3		m_LeftHandEndPos;		//左手の元の位置.

	D3DXQUATERNION	m_StartQuat;			//開始時の回転.

private:
	CPlayerHandAttackState() = delete;	//デフォルトコンストラクタ禁止.
	CPlayerHandAttackState(const CPlayerHandAttackState&) = delete;
	CPlayerHandAttackState& operator = (const CPlayerHandAttackState& rhs) = delete;

};