#pragma once

#include "CPlayerState.h"

class CPlayerKnockback
	: public CPlayerState
{
public:
	CPlayerKnockback();
	~CPlayerKnockback() override;

	//状態が始めるときだけ呼び出される.
	void Enter(CPlayer& pPlayer) override;
	//状態が終わるときだけ呼び出される.
	void Exit(CPlayer& pPlayer) override;
	//この状態の間は毎フレーム呼び出される.
	void Update(CPlayer& pPlayer) override;
	//入力されたら切り替える処理を呼び出す.
	void Handle(CPlayer& pPlayer, int inputKey) override;

	std::string GetStateName() const override;

private:
	D3DXVECTOR3		m_UpHandOffset;			//上に調整.
	D3DXVECTOR3		m_StartRightAxis;		//開始時の右軸.
	D3DXVECTOR3		m_Velocity;				//初速度.	

	float			m_StartTime;			//開始時間.
	float			m_EndTime;				//終了時間.

	float			m_Gravity;				//重力.
	float			m_RotateSpeed;			//回転する速度.
	float			m_CurrentTiltAngle;		//現在の傾き角度.
	float			m_TiltAngleMax;			//最大の傾き角度.

	bool			m_IsPhase;

	D3DXQUATERNION	m_StartQuat;			//開始時の回転.

};