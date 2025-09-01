#pragma once

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/PlayerState/ActionState/CActionState.h"

class CPlayerPickupItem
	: public CActionState
{
public:
	CPlayerPickupItem();
	~CPlayerPickupItem() override;

	//状態が始めるときだけ呼び出される.
	void Enter(CPlayer& pPlayer) override;
	//状態が終わるときだけ呼び出される.
	void Exit(CPlayer& pPlayer) override;
	//この状態の間は毎フレーム呼び出される.
	void Update(CPlayer& pPlayer) override;

private:
	D3DXVECTOR3		m_RightHandPos;			//右手の位置.
	D3DXVECTOR3		m_LeftHandPos;			//左手の位置.

	float			m_CenterOffset;			//中心に寄りに調整.

	float			m_StartTime;			//開始時間.
	float			m_EndTime;				//終了時間.

	float			m_CurrentTiltAngle;		//現在の傾き角度.
	float			m_TiltAngleMax;			//最大の傾き角度.
	float			m_PhaseSplit;			//傾きの変わり目.

	float			m_CurrentForwardHandSpeed;	//手の現在の速度.
	float			m_StoppingForwardHandSpeed;	//止まっているときの手の前に進む速度.
	float			m_MovingForwardHandSpeed;	//動いているときの手の前に進む速度.
	float			m_DownHandSpeed;			//手の下がる速度.
	
	D3DXQUATERNION	m_StartQuat;			//開始時の回転.
};