#pragma once

#include "PlayerBase/CPlayerBase.h"

#include "PlayerBase/PlayerManager/CPlayerManager.h"
#include "Item/ItemManager/ItemManager.h"

/***********************************************************************
*	プレイヤーAIクラス.							制作者 [甲把]
**/
class CPlayerAI
	: public CPlayerBase	//プレイヤーベースクラスを継承.
{
public:
//======================================================================
//		構造体.
//======================================================================
	//近くのオブジェクト情報.
	struct NearbyObject
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

	void AutomaticMovement(D3DXVECTOR3 targetDir);

	//プレイヤーを設定する.
	void SetPlayerManager(CPlayerManager* manager) { m_pPlayerManager = manager; }
	//アイテムを設定する.
	void SetItemManager(ItemManager* manager) { m_pItemManager = manager; }

protected:
	//--- 近くのプレイヤーを探索 ---.
	void FindNearbyPlayers();

	//--- 近くのアイテムを探索 ---.
	void FindNearbyItems();

protected:
	CPlayerManager*	m_pPlayerManager;	//プレイヤー.
	ItemManager*	m_pItemManager;		//アイテム.

	D3DXVECTOR3		m_CurrentDir;		//現在の進行方向.
	D3DXVECTOR3		m_PreviousDiff;		//以前との差.

	NearbyObject	m_NearbyPlayers;	//近くのプレイヤー.
	NearbyObject	m_NearbyItems;		//近くのアイテム.

	float m_Sqrt;
};