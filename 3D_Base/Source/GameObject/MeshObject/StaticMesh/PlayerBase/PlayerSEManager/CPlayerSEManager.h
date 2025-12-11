#pragma once

#include "PlayerBase/CPlayerBase.h"
#include "PlayerBase/PlayerManager/CPlayerManager.h"

/***********************************************************************
*   プレイヤーSEマネージャークラス.				制作者 [甲把]
**/
class CPlayerSEManager
{
public:
	enum PlayerSEList
	{
		Pickup,
		AttackHand,
		Whiff,
		Pushed,
		Knockback,
		Knockdown,
		Falling,

		Max
	};

public:
	CPlayerSEManager(std::unique_ptr<CPlayerManager>& playerManager);
	~CPlayerSEManager();

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//--- 効果音の設定 ---.
	void SetPlayerSE();
	//--- 更新処理 ---.
	void Update();

private:
//======================================================================
// 	   内部で呼び出す関数.
//======================================================================

private:
//======================================================================
// 	   内部で使用する変数.
//======================================================================
	std::unique_ptr<CPlayerManager>&	m_pPlayerManager;	//プレイヤーマネージャー.

	std::vector<bool>		m_IsSE;		//SEが鳴ったか.
};