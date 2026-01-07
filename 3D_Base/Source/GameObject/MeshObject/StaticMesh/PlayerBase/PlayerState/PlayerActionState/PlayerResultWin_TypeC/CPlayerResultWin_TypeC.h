#pragma once

#include "PlayerBase/PlayerState/CPlayerState.h"

/***********************************************************************
*	プレイヤーが勝った時の状態クラス.
**/
class CPlayerResultWin_TypeC
	: public CPlayerState	//プレイヤー状態を継承.
{
public:
	CPlayerResultWin_TypeC(CPlayerBase& pPlayer);
	~CPlayerResultWin_TypeC();

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//--- 状態の開始時に呼び出す ---.
	void Enter() override;
	//--- 状態の終了時に呼び出す ---.
	void Exit() override;
	//--- この状態の間に呼び出す ---.
	void Update() override;

private:
//======================================================================
// 	   内部で使用する変数.
//======================================================================
	float			m_StartTime;			//開始時間.
	float			m_EndTime;				//終了時間.

	float			m_HandLaps;				//手の周回.
	float			m_HandWidth;			//手の振り幅.

	D3DXVECTOR3		m_RightHandPos;			//右手の位置.
	D3DXVECTOR3		m_LeftHandPos;			//左手の位置.

	D3DXVECTOR3		m_RightHandEndPos;		//右手の目的の位置.
	D3DXVECTOR3		m_LeftHandEndPos;		//左手の目的の位置.

private:
//======================================================================
// 	   呼び出しを禁止するコンストラクタ.
//======================================================================
	CPlayerResultWin_TypeC() = delete;	//デフォルトコンストラクタ削除.
	CPlayerResultWin_TypeC(const CPlayerResultWin_TypeC&) = delete;
	CPlayerResultWin_TypeC& operator = (const CPlayerResultWin_TypeC& rhs) = delete;

};