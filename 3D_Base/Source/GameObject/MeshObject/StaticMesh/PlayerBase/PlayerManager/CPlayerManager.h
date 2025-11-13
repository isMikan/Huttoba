#pragma once

#include "PlayerBase/CPlayerBase.h"

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
	//--- 構築関数 ---.
	void Create();
	//--- 読込関数 ---.
	void LoadData();
	//--- 破棄関数 ---.
	void Destroy(CPlayerBase* player);
	//--- 更新関数 ---.
	void Update();
	//--- 描画関数 ---.
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera);

	//--- リザルトシーンの設定 ---.
	void ResultPlayerCreate();

	//今は実際使うかわからないのでコメント化.
	//エフェクトを表示するための関数.
	//void ManageEffectLaser(static::EsHandle hEffect);

	//一人のプレイヤーを取得.
	CPlayerBase* GetPlayer(int id) { return m_pPlayers[id].get(); }
	std::vector<std::unique_ptr<CPlayerBase>>& GetPlayer() { return m_pPlayers; }

private:
//======================================================================
// 	   内部で呼び出す関数.
//======================================================================
	//--- キャラクターの色を設定する関数 ---.
	ObjectColor CharacterColorSettings(int index);

	//--- 初期位置と方向を設定する関数 ---.
	void InitialSettings(int index);

private:
//======================================================================
// 	   内部で使用する変数.
//======================================================================
	std::vector<std::unique_ptr<CPlayerBase>>		m_pPlayers;		//プレイヤー.

	float			m_CreateTime;		//作成された時間.
	float			m_ReadyTime;		//準備OKの時間.

};