#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/CPlayerState.h"

/************************************************************
*	プレイヤーがアイテム未所持の攻撃を空振りする状態クラス.
**/
class CPlayerHandWhiffState
	: public CPlayerState	//プレイヤー状態継承クラス.
{
public:
	CPlayerHandWhiffState();
	~CPlayerHandWhiffState();

	//状態が始めるときだけ呼び出される.
	void Enter(CPlayerBase& pPlayerBase) override;
	//状態が終わるときだけ呼び出される.
	void Exit(CPlayerBase& pPlayerBase) override;
	//この状態の間は毎フレーム呼び出される.
	void Update(CPlayerBase& pPlayerBase) override;

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
};
