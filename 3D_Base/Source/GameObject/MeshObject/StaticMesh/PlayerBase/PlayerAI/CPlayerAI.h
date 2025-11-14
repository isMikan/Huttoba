#pragma once

#include "PlayerBase/CPlayerBase.h"

#include "PlayerBase/PlayerManager/CPlayerManager.h"

/***********************************************************************
*	プレイヤーAIクラス.
**/
class CPlayerAI
	: public CPlayerBase	//プレイヤーベースクラスを継承.
{
public:
	CPlayerAI(int index);	//引数:プレイヤー番号.
	virtual ~CPlayerAI() override;

//======================================================================
//		外部で呼び出す関数.
//======================================================================
	//--- 毎フレームの動作 ---.
	virtual void Update() override;
	//--- 毎フレームの描画 ---.
	virtual void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera ) override;

	void AutomaticMovement();

	//プレイヤーを設定する.
	void SetPlayerManager(CPlayerManager* manager) { m_pPlayerManager = manager; }

protected:
	//--- 近くのプレイヤーを追いかける ---.
	void FollowPlayerNearby();

protected:
	CPlayerManager*	m_pPlayerManager;		//プレイヤー.

	D3DXVECTOR3		m_CurrentDir;	//現在の進行方向.
	D3DXVECTOR3		m_TargetDir;	//目標の進行方向.

	D3DXVECTOR3		m_PreviousDiff;	//以前との差.

	float m_Sqrt;
};