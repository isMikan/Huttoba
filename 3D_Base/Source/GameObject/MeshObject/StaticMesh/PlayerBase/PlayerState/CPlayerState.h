#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerObserver/IPlayerObserver.h"

class CPlayerBase;

/****************************************
*	プレイヤー状態継承クラス.
**/
class CPlayerState
	: public IPlayerObserver	//オブサーバを継承.
{
public:
	CPlayerState(CPlayerBase& pPlayer);
	virtual ~CPlayerState();

	//--- 通知 ---.
	virtual void OnNotify(IPlayerObserver::PlayerEvent event) override {};

	//--- 状態の開始時に呼び出す関数 ---.
	virtual void Enter() = 0;
	//--- 状態の終了時に呼び出す関数 ---.
	virtual void Exit() = 0;
	//--- この状態の間に呼び出す関数 ---.
	virtual void Update() = 0;

protected:
	CPlayerBase&	m_pPlayer;		//プレイヤーを参照.

protected:
	CPlayerState() = delete;	//デフォルトコンストラクタ禁止.
	CPlayerState(const CPlayerState&) = delete;
	CPlayerState& operator = (const CPlayerState& rhs) = delete;

};