#pragma once

#include "PlayerBase/CPlayerBase.h"
#include "Item/ItemManager/ItemManager.h"

class CGroundManager;

/***********************************************************************
*   プレイヤーマネージャークラス.				制作者 [甲把]
**/
class CPlayerManager
{
public:
	CPlayerManager();
	~CPlayerManager();

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//--- 読込処理 ---.
	void LoadData();
	//--- 描画処理 ---.
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera);

//=== 各シーンの構築関数 ===.
	//--- 準備画面 ---.
	void StandbyPlayerCreate();
	//--- メイン ---.
	void MainPlayerCreate(ItemManager* itemManager, CGroundManager* groundManager);
	//--- リザルト ---.
	void ResultPlayerCreate();

//=== 各シーンの更新関数 ===.
	//--- 更新処理 ---.
	void Update();
	//--- タイトル ---.
	void TitlePlayerUpdate();
	//--- 準備 ---.
	void StandbyPlayerUpdate();
	//--- メイン ---.
	void MainPlayerUpdate();
	//--- リザルト ---.
	void ResultPlayerUpdate();

	//一人のプレイヤーを取得.
	CPlayerBase* GetPlayer(int id) { return m_pPlayers[id].get(); }
	std::vector<std::unique_ptr<CPlayerBase>>& GetPlayer() { return m_pPlayers; }

private:
//======================================================================
// 	   内部で呼び出す関数.
//======================================================================
	//--- 構築処理 ---.
	void Create();
	//--- 破棄処理 ---.
	void Destroy(CPlayerBase* player);

	//--- キャラクターの色を設定する ---.
	ObjectColor CharacterColorSettings(int index);

	//--- 初期位置と方向を設定する ---.
	void InitialSettings(int index);

private:
//======================================================================
// 	   内部で使用する変数.
//======================================================================
	std::vector<std::unique_ptr<CPlayerBase>>		m_pPlayers;		//プレイヤー.

	std::vector<int>		m_LivingOrder;		//最後に並ぶ順.

	float					m_InitialSetPosY;	//y軸の初期位置.
};