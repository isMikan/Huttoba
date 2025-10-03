#pragma once

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/PlayerState/CPlayerState.h"

/**************************************************
*	プレイヤーの吹き飛ばされた状態クラス.
**/
class CPlayerKnockbackState
	: public CPlayerState
{
public:
	CPlayerKnockbackState();
	~CPlayerKnockbackState() override;

	//状態が始めるときだけ呼び出される.
	void Enter(CPlayer& pPlayer) override;
	//状態が終わるときだけ呼び出される.
	void Exit(CPlayer& pPlayer) override;
	//この状態の間は毎フレーム呼び出される.
	void Update(CPlayer& pPlayer) override;

private:
	D3DXVECTOR3		m_UpHandOffset;			//上に調整.
	D3DXVECTOR3		m_StartRightAxis;		//開始時の右軸.
	D3DXVECTOR3		m_Velocity;				//初速度.	

	float			m_StartTime;			//開始時間.
	float			m_EndTime;				//終了時間.

	float			m_Gravity;				//重力.
	float			m_RotateSpeed;			//回転する速度.
	float			m_CurrentTiltAngle;		//現在の傾き角度.

	D3DXQUATERNION	m_StartQuat;			//開始時の回転.
};