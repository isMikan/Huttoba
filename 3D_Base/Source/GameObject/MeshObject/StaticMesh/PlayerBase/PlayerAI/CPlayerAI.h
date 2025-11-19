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
//======================================================================
//		構造体.
//======================================================================
	//近くのプレイヤー情報.
	struct NearbyPlayers
	{
		D3DXVECTOR3		dir;	//方向.
		float			sqrt;	//距離.
	};

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
	//--- 近くのプレイヤーを探索 ---.
	void FindNearbyPlayers();

protected:
	CPlayerManager*	m_pPlayerManager;	//プレイヤー.

	D3DXVECTOR3		m_CurrentDir;		//現在の進行方向.
	D3DXVECTOR3		m_PreviousDiff;		//以前との差.

	NearbyPlayers	m_NearbyPlayers;	//近くのプレイヤー

	float m_Sqrt;
};